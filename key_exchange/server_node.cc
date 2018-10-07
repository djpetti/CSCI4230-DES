#include "server_node.h"

#include <stdio.h>
#include <string.h>

#include "constants.h"

namespace hw1 {
namespace key_exchange {

ServerNode::ServerNode(const uint8_t *key, uint8_t id)
    : transfer::common::Server(key, kChunkSize), id_(id), kdc_key_(key) {}

bool ServerNode::HandleConnection() {
  // First, wait for the connection.
  if (!transfer::common::Server::WaitForConnection()) {
    return false;
  }

  // Now, perform the handshake.
  if (!DoHandshake()) {
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
    // Disconnect.
    CleanUp();
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

void ServerNode::CleanUp() {
  transfer::common::Server::CleanUp();

  // Our implementation of CleanUp() also resets the encryption key back to the
  // KDC master key.
  SetKey(kdc_key_);
}

}  // namespace key_exchange
}  // namespace hw1
