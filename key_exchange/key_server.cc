#include "key_server.h"

#include <stdio.h>
#include <string.h>
#include <sys/random.h>

#include "constants.h"

namespace hw1 {
namespace key_exchange {
namespace {

// Dummy key to use for initialization.
const uint8_t kDummyKey[] = {0, 0};

}  // namespace

// Initialize with a dummy key. (We'll set the correct key on-the-fly when we
// need to send a message to the clients.)
KeyServer::KeyServer()
    : transfer::common::Server(kDummyKey, kKeyMessageSize),
      des_(kDummyKey) {}

void KeyServer::AddClient(uint8_t id, const uint8_t *key) {
  printf("Adding client with ID %u.\n", id);

  // Add it to the map.
  Key my_key;
  memcpy(my_key.Key, key, 2);
  client_keys_[id] = my_key;
}

void KeyServer::HandleConnection() {
  // First, wait for a client to connect.
  WaitForConnection();

  // Handle transactions until disconnect.
  while (HandleTransaction())
    ;

  // Close the connection nicely if we have to.
  if (ClientConnected()) {
    CleanUp();
  }
}

bool KeyServer::HandleTransaction() {
  // Wait for the first message.
  uint8_t client_a;
  uint8_t client_b;
  uint32_t nonce;
  if (!GetFirstMessage(&client_a, &client_b, &nonce)) {
    return false;
  }

  // Generate the key and send it.
  if (!GenerateAndSendKey(client_a, client_b, nonce)) {
    return false;
  }

  return true;
}

bool KeyServer::GetFirstMessage(uint8_t *id_a, uint8_t *id_b, uint32_t *nonce) {
  // The length comes from 2 IDs and one nonce.
  const uint8_t length = 1 + 1 + 4;
  // Read the message into the buffer.
  char *buffer;
  const uint32_t actual_read = ReceiveChunk(&buffer, length);
  if (actual_read != length) {
    // Did not read an entire message.
    return false;
  }

  // Separate out the three parts of the message.
  memcpy(id_a, buffer, 1);
  ++buffer;
  memcpy(id_b, buffer, 1);
  ++buffer;
  memcpy(nonce, buffer, 4);

  printf("Got key request from %u to %u with nonce %u.\n", *id_a, *id_b,
         *nonce);

  if (!VerifyNonce(*id_a, *nonce)) {
    // Invalid nonce.
    return false;
  }

  return true;
}

bool KeyServer::GenerateAndSendKey(uint8_t id_a, uint8_t id_b, uint32_t nonce) {
  if (key_a == client_keys_.end()) {
    // We don't know this client. We have to perform key exchange before doing
    // anything else.
    printf("Unknown node %u, performing key exchange.\n", id_a);
    if (!DoKeyExchange(id_a)) {
      return false;
    }

    // Find it again now that it's in there.
    key_a = client_keys_.find(id_a);
    // The other one might be in there as well.
    key_b = client_keys_.find(id_b);
  }
  if (key_b == client_keys_.end()) {
    // We always have to be familiar with the destination.
    printf("ERROR: Unknown destination node %u.\n", id_b);
    return false;
  }
  if (id_a == id_b) {
    // This is a special case when we just want to perform a key exchange and
    // nothing else.
    printf("Aborting after key exchange.\n");
    return true;
  }

  // Now, generate a new session key.
  uint8_t session_key[2];
  if (!GenerateKey(session_key)) {
    // Key generation failed.
    return false;
  }

  // Make envelope for client B. It should contain one client ID, one session
  // key, and one nonce.
  const uint8_t envelope_size = 2 + 1 + 4;
  uint8_t plain_envelope[envelope_size];
  memcpy(plain_envelope, session_key, 2);
  memcpy(plain_envelope + 2, &id_a, 1);
  const uint32_t session_nonce = nonce_generator_.Generate();
  memcpy(plain_envelope + 3, &session_nonce, 4);
  printf("Generated session key 0x%X 0x%X, nonce %u.\n", session_key[0],
         session_key[1], session_nonce);

  // Encrypt the envelope.
  uint8_t envelope[envelope_size];
  des_.SetKey(key_b->second.Key);
  des_.Encrypt((const char *)plain_envelope, envelope_size, (char *)envelope);

  // Generate a reply.
  uint8_t offset = 0;
  memcpy(plain_chunk_buffer_, session_key, 2);
  offset += 2;
  memcpy(plain_chunk_buffer_ + offset, &id_b, 1);
  ++offset;
  memcpy(plain_chunk_buffer_ + offset, &nonce, 4);
  offset += 4;
  memcpy(plain_chunk_buffer_ + offset, envelope, envelope_size);
  offset += envelope_size;

  // Set the correct key for encryption.
  SetKey(key_a->second.Key);

  // Send the reply.
  if (EncryptAndSendChunk(plain_chunk_buffer_, offset) != offset) {
    // Failed to send reply.
    return false;
  }

  return true;
}

bool KeyServer::GenerateKey(uint8_t *key) {
  // Use Linux feature to generate a crypto-quality random number.
  const int got_bytes = getrandom(key, 2, 0);
  if (got_bytes != 2) {
    perror("ERROR");
    return false;
  }

  return true;
}

bool KeyServer::DoKeyExchange(uint8_t node_id) {
  // First, generate a private key.
  const uint64_t private_key = key_gen_.GeneratePrivateKey();
  // Compute the corresponding public key.
  const uint64_t public_key = key_gen_.ComputePublicKey(private_key);
  printf("Computed public key: %lu\n", public_key);

  // Send the public key.
  if (!SendChunk((const char *)&public_key, 8)) {
    return false;
  }

  // Wait for them to send us their public key.
  char *buffer;
  if (!ReceiveChunk(&buffer, 8)) {
    return false;
  }
  uint64_t other_public_key;
  memcpy((uint8_t *)&other_public_key, buffer, 8);
  printf("Received public key: %lu\n", other_public_key);

  // Generate the master key.
  uint64_t session_key =
      key_gen_.ComputeSessionKey(other_public_key, private_key);
  // Truncate to 10 bits for use in DES.
  Key des_key;
  des_key.Key[0] = session_key & 0xFF;
  des_key.Key[1] = (session_key >> 8) & 0x03;
  printf("Computed master key: 0x%X 0x%X\n", des_key.Key[0], des_key.Key[1]);

  // Save the key for the node.
  client_keys_[node_id] = des_key;

  return true;
}

}  // namespace key_exchange
}  // namespace hw1
