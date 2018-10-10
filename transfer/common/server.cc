#include "server.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>

namespace hw1 {
namespace transfer {
namespace common {
namespace {

// Binds a socket to a port.
// Args:
//  socket: The socket fd.
//  port: The port to bind to.
// Returns:
//  True if binding succeeded, false otherwise.
static bool bindSocket(int socket, int port) {
  // Set up the address specifier.
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);

  // Bind it.
  if (bind(socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    // Binding failed.
    perror("ERROR");
    return false;
  }

  return true;
}

}  // namespace

Server::Server(const uint8_t *key, uint32_t chunk_size)
    : SecureNode(key, chunk_size) {}

Server::~Server() {
  // Cleanup the client and close the server.
  CleanUp();
  close(socket_);
}

bool Server::Listen(uint16_t port) {
  // Create the socket.
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_ < 0) {
    perror("ERROR");
    return false;
  }

  // Bind it to the port.
  if (!bindSocket(socket_, port)) {
    return false;
  }

  // Start listening on the socket.
  if (listen(socket_, 5) < 0) {
    perror("ERROR");
    return false;
  }

  printf("Server listening on port %u.\n", port);

  return true;
}

bool Server::WaitForConnection() {
  // First, accept the client.
  struct sockaddr_in client_addr;
  int client_len = sizeof(client_addr);
  client_sock_ = accept(socket_, (struct sockaddr *)&client_addr,
                        (socklen_t *)&client_len);
  if (client_sock_ < 0) {
    // Accept failed for some reason.
    perror("ERROR");
    return false;
  }

  printf("Received connection from %s.\n", inet_ntoa(client_addr.sin_addr));

  return true;
}

uint32_t Server::ReceiveChunk(char **buffer) {
  return SecureNode::ReceiveChunk(client_sock_, buffer);
}

uint32_t Server::ReceiveChunk(char **buffer, uint32_t length) {
  return SecureNode::ReceiveChunk(client_sock_, buffer, length);
}

uint32_t Server::ReceiveAndDecryptChunk(char **buffer) {
  return SecureNode::ReceiveAndDecryptChunk(client_sock_, buffer);
}

uint32_t Server::ReceiveAndDecryptChunk(char **buffer, uint32_t length) {
  return SecureNode::ReceiveAndDecryptChunk(client_sock_, buffer, length);
}

uint32_t Server::SendChunk(const char *buffer, uint32_t length) {
  return SecureNode::SendChunk(client_sock_, buffer, length);
}

uint32_t Server::EncryptAndSendChunk(const char *buffer, uint32_t length) {
  return SecureNode::EncryptAndSendChunk(client_sock_, buffer, length);
}

bool Server::ClientConnected() {
  return client_sock_ != -1;
}

void Server::CleanUp() {
  printf("Client disconnected.\n");
  close(client_sock_);
  // Indicate that no client is connected.
  client_sock_ = -1;
}

}  // namespace common
}  // namespace transfer
}  // namespace hw1
