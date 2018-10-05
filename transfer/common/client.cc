#include "client.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>

namespace hw1 {
namespace transfer {
namespace common {

Client::Client(const uint8_t *key, uint32_t chunk_size)
    : SecureNode(key, chunk_size) {}

Client::~Client() {
  // Close the socket.
  close(socket_);
}

bool Client::Connect(const char *server, uint16_t port) {
  // Create the socket.
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_ < 0) {
    // Socket creation failed.
    perror("ERROR");
    return false;
  }

  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);

  // Convert IP address to binary form.
  if (inet_pton(AF_INET, server, &server_address.sin_addr) <= 0) {
    perror("ERROR");
    return false;
  }

  // Connect to the server.
  if (connect(socket_, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    perror("ERROR");
    return false;
  }

  return true;
}

uint32_t Client::ReceiveChunk(char **buffer) {
  return SecureNode::ReceiveChunk(socket_, buffer);
}

uint32_t Client::ReceiveChunk(char **buffer, uint32_t length) {
  return SecureNode::ReceiveChunk(socket_, buffer, length);
}

uint32_t Client::ReceiveAndDecryptChunk(char **buffer) {
  return SecureNode::ReceiveAndDecryptChunk(socket_, buffer);
}

uint32_t Client::ReceiveAndDecryptChunk(char **buffer,
                                                uint32_t length) {
  return SecureNode::ReceiveAndDecryptChunk(socket_, buffer, length);
}

uint32_t Client::SendChunk(const char *buffer, uint32_t length) {
  return SecureNode::SendChunk(socket_, buffer, length);
}

uint32_t Client::EncryptAndSendChunk(const char *buffer, uint32_t length) {
  return SecureNode::EncryptAndSendChunk(socket_, buffer, length);
}

}  // namespace common
}  // namespace transfer
}  // namespace hw1
