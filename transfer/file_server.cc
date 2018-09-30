#include "file_server.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>

#include "constants.h"

namespace hw1 {
namespace transfer {

Server::Server(const uint8_t *key) : common::Server(key, kChunkSize) {}

bool Server::HandleConnection() {
  // Wait for a client to connect.
  if (!WaitForConnection()) {
    return false;
  }

  // Receive data from the client.
  while (ClientConnected()) {
    if (!HandleMessage()) {
      // Message receive failure.
      if (ClientConnected()) {
        CleanUp();
      }
      return false;
    }
  }

  return true;
}

bool Server::HandleMessage() {
  // First, receive the message header.
  char *buffer;
  const uint32_t bread = ReceiveChunk(&buffer, kMessageHeaderSize);
  if (bread == 0) {
    // Read error.
    return false;
  }
  if (bread != kMessageHeaderSize) {
    // The header should always be exactly the same size.
    fprintf(stderr, "ERROR: Received improper message header.\n");
    return false;
  }

  // Make sure we have a trailing NULL somewhere before the size.
  buffer[kMessageHeaderSize - sizeof(uint32_t) - 1] = '\0';

  // Get the file name and message length. The filename should have a trailing
  // NULL, so we can just look at the start of the buffer.
  const char *filename = buffer;
  uint32_t file_size;
  memcpy((uint8_t *)&file_size,
         buffer + kMessageHeaderSize - sizeof(uint32_t),
         sizeof(uint32_t));

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
    char *buffer;
    const uint32_t actual_read = ReceiveAndDecryptChunk(&buffer, max_read);
    if (actual_read == 0) {
      // Read error.
      return false;
    }

    received_size += actual_read;

    // Write the chunk.
    if (fwrite(buffer, 1, actual_read, file) != actual_read) {
      // Failed to write everything.
      fprintf(stderr, "ERROR: Failed to write file chunk.");
      return false;
    }
  }

  fclose(file);

  return true;
}

}  // namespace transfer
}  // namespace hw1
