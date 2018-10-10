#include "server_node.h"

#include <stdio.h>
#include <string.h>

#include "constants.h"

namespace hw1 {
namespace key_exchange {
namespace {

// Dummy key we use when we set up the client. We'll set the right key later
// when we have it.
const uint8_t kDummyKey[] = {0, 0};

}  // namespace

ServerNode::ServerNode(const char *kdc_address, uint16_t kdc_port, uint8_t id)
    : transfer::common::Server(kDummyKey, kChunkSize),
      id_(id),
      key_manager_(kdc_address, kdc_port, id, &nonce_generator_) {}

bool ServerNode::HandleConnection() {
  // First, set the proper master key.
  uint8_t kdc_key[2];
  if (!key_manager_.GetMasterKey(kdc_key)) {
    // Key exchange failed.
    CleanUp();
    return false;
  }
  SetKey(kdc_key);

  // Now, wait for the connection.
  if (!transfer::common::Server::WaitForConnection()) {
    CleanUp();
    return false;
  }

  // Now, perform the handshake.
  if (!DoHandshake()) {
    CleanUp();
    return false;
  }

  // Handle transactions until the client disconnects.
  while (HandleTransaction());

  // Clean up nicely if we have to.
  if (ClientConnected()) {
    CleanUp();
  }

  return true;
}

bool ServerNode::DoHandshake() {
  // Receive the handshake message.
  char *buffer;
  if (!ReceiveAndDecryptChunk(&buffer, kHandshakeMessageSize)) {
    // Failed to receive the handshake message.
    fprintf(stderr, "Handshake with client failed.\n");
    return false;
  }

  // Read out the session key and client ID.
  uint8_t session_key[2];
  uint8_t client_id;
  uint32_t nonce;
  memcpy(&session_key, buffer, 2);
  memcpy(&client_id, buffer + 2, 1);
  memcpy(&nonce, buffer + 3, 4);

  printf("Connected to client %u with key 0x%X 0x%X and nonce %u.\n", client_id,
         session_key[0], session_key[1], nonce);

  if (!VerifyNonce(client_id, nonce)) {
    // Nonce is invalid.
    return false;
  }

  // Set it to encrypt further transactions with the session key.
  SetKey(session_key);

  return true;
}

bool ServerNode::HandleTransaction() {
  // Receive the size of the next message.
  char *buffer;
  if (!ReceiveAndDecryptChunk(&buffer, 2)) {
    return false;
  }

  // Read the size.
  uint16_t message_size;
  memcpy(&message_size, buffer, 2);
  printf("Receiving message with size: %u\n", message_size);

  // Receive the actual message.
  if (!ReceiveAndDecryptChunk(&buffer, message_size)) {
    return false;
  }

  printf("Received message: %s\n", buffer);

  return true;
}

}  // namespace key_exchange
}  // namespace hw1
