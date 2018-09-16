#include "client.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>

#include "constants.h"

namespace hw1 {
namespace transfer {

Client::Client(const uint8_t *key) : encryptor_(key) {
  // Allocate buffers.
  header_buffer_ = new char[kMessageHeaderSize];
  chunk_buffer_ = new char[kChunkSize];
  cipher_chunk_buffer_ = new char[kChunkSize];
}

Client::~Client() {
  // Deallocate buffers.
  delete[] header_buffer_;
  delete[] chunk_buffer_;
  delete[] cipher_chunk_buffer_;

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

bool Client::SendFile(const char *filename) {
  printf("Sending file to server: %s\n", filename);

  // Open the file for reading.
  FILE *file = fopen(filename, "rb");
  if (!file) {
    // Failed to open the file.
    perror("ERROR");
    return false;
  }

  // Send the header information for the file.
  const uint32_t file_size = SendHeader(filename, file);
  if (!file_size) {
    return false;
  }

  // How many bytes we have left to send.
  uint32_t bytes_remaining = file_size;
  while (bytes_remaining > 0) {
    // Read the next chunk from the file.
    const uint32_t max_read = ::std::min(kChunkSize, bytes_remaining);
    const uint32_t actual_read = fread(chunk_buffer_, 1, max_read, file);
    if (actual_read != max_read) {
      printf("Unexpected failure to read from file.");
      return false;
    }

    bytes_remaining -= actual_read;

    // Encrypt the chunk.
    encryptor_.Encrypt(chunk_buffer_, actual_read, cipher_chunk_buffer_);

    // Send the chunk.
    if (!send(socket_, cipher_chunk_buffer_, actual_read, 0)) {
      printf("Unexpected failure to send data.");
      return false;
    }
  }

  return true;
}

uint32_t Client::SendHeader(const char *filename, FILE *file) {
  // Determine the size of the file.
  fseek(file, 0L, SEEK_END);
  const uint32_t size = ftell(file);
  rewind(file);

  printf("Will send %u bytes.\n", size);

  // Format the header.
  strncpy(header_buffer_, filename, kMessageHeaderSize);
  // Make sure we have a null terminator somewhere.
  header_buffer_[kMessageHeaderSize - sizeof(uint32_t) - 1] = '\0';

  // Append the file size.
  memcpy(header_buffer_ + kMessageHeaderSize - sizeof(uint32_t),
         (const uint8_t *)&size, sizeof(uint32_t));

  // Send the header.
  const uint32_t bsent = send(socket_, header_buffer_, kMessageHeaderSize, 0);
  if (bsent != kMessageHeaderSize) {
    // Failed to send the buffer.
    perror("ERROR");
    return 0;
  }

  return size;
}

}  // namespace transfer
}  // namespace hw1
