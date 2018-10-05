#include "secure_node.h"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace hw1 {
namespace transfer {
namespace common {

SecureNode::SecureNode(const uint8_t *key, uint32_t chunk_size)
    : chunk_size_(chunk_size), des_(key) {
  // Allocate the buffers.
  chunk_buffer_ = new char[chunk_size_];
  plain_chunk_buffer_ = new char[chunk_size_];
}

SecureNode::~SecureNode() {
  // Deallocate the buffers.
  delete[] chunk_buffer_;
  delete[] plain_chunk_buffer_;
}

void SecureNode::SetKey(const uint8_t *key) {
  des_.SetKey(key);
}

uint32_t SecureNode::ReceiveChunk(int socket, char **buffer) {
  return ReceiveChunk(socket, buffer, chunk_size_);
}

uint32_t SecureNode::ReceiveAndDecryptChunk(int socket, char **buffer) {
  return ReceiveAndDecryptChunk(socket, buffer, chunk_size_);
}

uint32_t SecureNode::ReceiveChunk(int socket, char **buffer, uint32_t length) {
  // Receive the next chunk.
  const int32_t actual_read =
      recv(socket, plain_chunk_buffer_, length, 0);
  if (actual_read < 0) {
    // Reading error.
    perror("ERROR");
    return 0;
  }
  if (actual_read == 0) {
    // Disconnected.
    return 0;
  }

  // Set the buffer.
  *buffer = plain_chunk_buffer_;

  return actual_read;
}

uint32_t SecureNode::ReceiveAndDecryptChunk(int socket, char **buffer,
                                        uint32_t length) {
  // Receive the next chunk.
  const int32_t actual_read = recv(socket, chunk_buffer_, length, 0);
  if (actual_read == -1) {
    // Reading error.
    perror("ERROR");
    return 0;
  }
  if (actual_read == 0) {
    // Disconnected..
    return 0;
  }

  // Decrypt the chunk.
  des_.Decrypt(chunk_buffer_, actual_read, plain_chunk_buffer_);

  // Set the buffer.
  *buffer = plain_chunk_buffer_;

  return actual_read;
}

uint32_t SecureNode::SendChunk(int socket, const char *buffer, uint32_t length) {
  // Send the chunk.
  return send(socket, buffer, length, 0);
}

uint32_t SecureNode::EncryptAndSendChunk(int socket, const char *buffer,
                                         uint32_t length) {
  // Encrypt the chunk.
  const uint32_t encrypt_length = ::std::min(length, chunk_size_);
  des_.Encrypt(buffer, encrypt_length, chunk_buffer_);

  // Send the chunk.
  return SendChunk(socket, chunk_buffer_, encrypt_length);
}

}  // namespace common
}  // namespace transfer
}  // namespace hw1
