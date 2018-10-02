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
    : chunk_size_(chunk_size), des_(key) {
  // Allocate the buffers.
  chunk_buffer_ = new char[chunk_size_];
  plain_chunk_buffer_ = new char[chunk_size_];
}

Server::~Server() {
  // Deallocate the buffers.
  delete[] chunk_buffer_;
  delete[] plain_chunk_buffer_;

  // Cleanup the client and close the server.
  CleanUp();
  close(socket_);
}

void Server::SetKey(const uint8_t *key) {
  des_.SetKey(key);
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

bool Server::ClientConnected() {
  return client_sock_ != -1;
}

uint32_t Server::ReceiveChunk(char **buffer) {
  return ReceiveChunk(buffer, chunk_size_);
}

uint32_t Server::ReceiveChunk(char **buffer, uint32_t length) {
  // Receive the next chunk.
  const uint32_t actual_read =
      recv(client_sock_, plain_chunk_buffer_, length, 0);
  if (actual_read == 0) {
    // Client disconnected.
    CleanUp();
    return 0;
  }

  // Set the buffer.
  *buffer = plain_chunk_buffer_;

  return actual_read;
}

uint32_t Server::ReceiveAndDecryptChunk(char **buffer) {
  return ReceiveAndDecryptChunk(buffer, chunk_size_);
}

uint32_t Server::ReceiveAndDecryptChunk(char **buffer, uint32_t length) {
  // Receive the next chunk.
  const uint32_t actual_read = recv(client_sock_, chunk_buffer_, length, 0);
  if (actual_read == 0) {
    // Client disconnected..
    CleanUp();
    return 0;
  }

  // Decrypt the chunk.
  des_.Decrypt(chunk_buffer_, actual_read, plain_chunk_buffer_);

  // Set the buffer.
  *buffer = plain_chunk_buffer_;

  return actual_read;
}

uint32_t Server::SendChunk(const char *buffer, uint32_t length) {
  // Send the chunk.
  return send(client_sock_, buffer, length, 0);
}

uint32_t Server::EncryptAndSendChunk(const char *buffer, uint32_t length) {
  // Encrypt the chunk.
  const uint32_t encrypt_length = ::std::min(length, chunk_size_);
  des_.Encrypt(buffer, encrypt_length, chunk_buffer_);

  // Send the chunk.
  return send(client_sock_, chunk_buffer_, encrypt_length, 0);
}

void Server::CleanUp() {
  printf("Client disconnected.\n");

  // Close the socket.
  close(client_sock_);
  // Indicate that no client is connected.
  client_sock_ = -1;
}

}  // namespace common
}  // namespace transfer
}  // namespace hw1
