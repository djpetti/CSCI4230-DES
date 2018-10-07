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

  return true;
}

bool KeyServer::GenerateAndSendKey(uint8_t id_a, uint8_t id_b, uint32_t nonce) {
  // First, we're going to have to find the keys for each client.
  const auto &key_a = client_keys_.find(id_a);
  const auto &key_b = client_keys_.find(id_b);
  if (key_a == client_keys_.end() || key_b == client_keys_.end()) {
    // We received an invalid client ID.
    return false;
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

}  // namespace key_exchange
}  // namespace hw1
