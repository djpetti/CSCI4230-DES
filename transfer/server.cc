#include "server.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>

namespace hw1 {
namespace transfer {
namespace {

// Size of message header, in bytes.
const uint8_t kMessageHeaderSize = 36;
// Size of a file chunk, in bytes.
const uint32_t kChunkSize = 1024;

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

Server::Server(const uint8_t *key) : decryptor_(key) {
  // Allocate the buffers.
  header_buffer_ = new char[kMessageHeaderSize];
  chunk_buffer_ = new char[kChunkSize];
  plain_chunk_buffer_ = new char[kChunkSize];
}

Server::~Server() {
  // Deallocate the buffers.
  delete[] header_buffer_;
  delete[] chunk_buffer_;
  delete[] plain_chunk_buffer_;
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

bool Server::HandleConnection() {
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

  // Receive data from the client.
  while (ClientConnected()) {
    if (!HandleMessage()) {
      // Message receive failure.
      CleanUp();
      return false;
    }
  }

  return true;
}

bool Server::ClientConnected() {
  return client_sock_ != -1;
}

bool Server::HandleMessage() {
  // First, receive the message header.
  const uint32_t bread =
      recv(client_sock_, header_buffer_, kMessageHeaderSize, 0);
  if (bread == 0) {
    // Client disconnected.
    CleanUp();
    return true;
  }
  if (bread != kMessageHeaderSize) {
    // The header should always be exactly the same size.
    fprintf(stderr, "ERROR: Received inproper message header.");
    return false;
  }

  // Make sure we have a trailing NULL somewhere before the size.
  header_buffer_[kMessageHeaderSize - sizeof(uint32_t) - 1] = '\0';

  // Get the file name and message length. The filename should have a trailing
  // NULL, so we can just look at the start of the buffer.
  const char *filename = header_buffer_;
  const uint32_t file_size =
      atoi(header_buffer_ + kMessageHeaderSize - sizeof(uint32_t));

  return ReceiveFile(filename, file_size);
}

bool Server::ReceiveFile(const char *filename, uint32_t file_size) {
  printf("Receiving file '%s' of size %u.\n", filename, file_size);

  // Open the file to begin with.
  FILE *file = fopen(filename, "ab");
  if (!file) {
    perror("ERROR");
    return false;
  }

  // How much data we have received so far.
  uint32_t received_size = 0;
  while (received_size < file_size) {
    // Receive the next chunk.
    const uint32_t max_read = ::std::min(file_size - received_size, kChunkSize);
    const uint32_t actual_read = recv(client_sock_, chunk_buffer_, max_read, 0);
    if (actual_read == 0) {
      // Unexpected disconnect.
      fprintf(stderr, "ERROR: Client disconnected during file transfer.");
      return false;
    }

    received_size += actual_read;

    // Decrypt the chunk.
    decryptor_.Decrypt(chunk_buffer_, actual_read, plain_chunk_buffer_);

    // Write the chunk.
    if (fwrite(plain_chunk_buffer_, 1, actual_read, file) != actual_read) {
      // Failed to write everything.
      fprintf(stderr, "ERROR: Failed to write file chunk.");
      return false;
    }
  }

  fclose(file);

  return true;
}

void Server::CleanUp() {
  printf("Client disconnected.\n");

  // Close the socket.
  close(client_sock_);
  // Indicate that no client is connected.
  client_sock_ = -1;
}

}  // namespace transfer
}  // namespace hw1
