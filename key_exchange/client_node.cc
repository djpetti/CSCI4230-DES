#include "client_node.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <algorithm>

#include "constants.h"

namespace hw1 {
namespace key_exchange {

ClientNode::ClientNode(const char *kdc_address, const uint16_t port,
                       const uint8_t *key, uint8_t id)
    : transfer::common::Client(key, kChunkSize),
      kdc_address_(kdc_address),
      kdc_port_(port),
      id_(id) {}

void ClientNode::AddNodeWithId(const char *address, uint8_t id) {
  printf("Adding node with address %s and ID %u.\n", address, id);

  node_ids_[address] = id;
}

bool ClientNode::Connect(const char *server, uint16_t port) {
  // Make sure that server is known to us.
  const auto &id_iter = node_ids_.find(server);
  if (id_iter == node_ids_.end()) {
    return false;
  }

  // When initiating a client connection, we first have to connect to the key
  // server and request a session key.
  if (!transfer::common::Client::Connect(kdc_address_, kdc_port_)) {
    // Failed to connect to the KDC.
    return false;
  }

  // Send the request.
  if (!RequestSessionKey(server)) {
    return false;
  }
  // Receive the response.
  uint8_t session_key[2];
  char node_envelope[kHandshakeMessageSize];
  if (!ReceiveSessionKey(session_key, node_envelope)) {
    return false;
  }

  // Now we have enough information to connect to the other node.
  Close();
  if (!transfer::common::Client::Connect(server, port)) {
    // Faled to connect to the other node.
    return false;
  }

  // Send the envelope with the session key.
  if (!SendChunk(node_envelope, kHandshakeMessageSize)) {
    return false;
  }

  // Use the new encryption key for all further communications with this node.
  SetKey(session_key);

  return true;
}

bool ClientNode::SendMessage(const char *message) {
  // Calculate the length + the terminating null.
  const uint16_t length = strlen(message) + 1;

  // Write everything into the chunk buffer.
  const uint32_t max_write = ::std::min((uint32_t)length, kChunkSize - 2);
  memcpy(plain_chunk_buffer_, &length, 2);
  memcpy(plain_chunk_buffer_ + 2, message, max_write);
  // Make sure we have a \0 somewhere in there.
  plain_chunk_buffer_[kChunkSize - 1] = '\0';

  // Send it to the client.
  return EncryptAndSendChunk(plain_chunk_buffer_, max_write + 2);
}

bool ClientNode::RequestSessionKey(const char *address) {
  // Format a request for a session key.
  const uint32_t nonce = nonce_generator_.Generate();
  const uint8_t other_id = node_ids_[address];
  char *write_at = plain_chunk_buffer_;
  memcpy(write_at++, &id_, 1);
  memcpy(write_at++, &other_id, 1);
  memcpy(write_at, &nonce, 4);
  write_at += 4;

  printf("Sending key request from %u to %u with nonce %u.\n", id_, other_id,
         nonce);

  // Send the request.
  if (!SendChunk(plain_chunk_buffer_, write_at - plain_chunk_buffer_)) {
    return false;
  }

  return true;
}

bool ClientNode::ReceiveSessionKey(uint8_t *session_key, char *node_envelope) {
  // Receive and decrypt the session key message.
  char *buffer;
  if (!ReceiveAndDecryptChunk(&buffer, kKeyMessageSize)) {
    // Failed to received the message.
    return false;
  }

  // Read the session key.
  memcpy(session_key, buffer, 2);
  // Read the envelope.
  memcpy(node_envelope, buffer + 7, kHandshakeMessageSize);

  printf("Will now use session key 0x%X 0x%X.\n", session_key[0],
         session_key[1]);

  return true;
}

}  // namespace key_exchange
}  // namespace hw1
