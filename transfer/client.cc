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

Client::Client(const uint8_t *key) : common::Client(key, kChunkSize) {}

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
    const uint32_t actual_read = fread(plain_chunk_buffer_, 1, max_read, file);
    if (actual_read != max_read) {
      fprintf(stderr, "Unexpected failure to read from file.\n");
      return false;
    }

    bytes_remaining -= actual_read;

    // Encrypt and send.
    if (EncryptAndSendChunk(plain_chunk_buffer_, actual_read) != actual_read) {
      fprintf(stderr, "Unexpected failure to send data.\n");
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
  strncpy(plain_chunk_buffer_, filename, kMessageHeaderSize);
  // Make sure we have a null terminator somewhere.
  plain_chunk_buffer_[kMessageHeaderSize - sizeof(uint32_t) - 1] = '\0';

  // Append the file size.
  memcpy(plain_chunk_buffer_ + kMessageHeaderSize - sizeof(uint32_t),
         (const uint8_t *)&size, sizeof(uint32_t));

  // Send the header.
  const uint32_t bsent = SendChunk(plain_chunk_buffer_, kMessageHeaderSize);
  if (bsent != kMessageHeaderSize) {
    // Failed to send the buffer.
    fprintf(stderr, "Failure to send header.\n");
    return 0;
  }

  return size;
}

}  // namespace transfer
}  // namespace hw1
