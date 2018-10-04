#include "secure_node.h"

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

uint32_t SecureNode::ReceiveChunk(char **buffer) {
  return ReceiveChunk(buffer, chunk_size_);
}

uint32_t SecureNode::ReceiveAndDecryptChunk(char **buffer) {
  return ReceiveAndDecryptChunk(buffer, chunk_size_);
}

uint32_t SecureNode::EncryptAndSendChunk(const char *buffer, uint32_t length) {
  // Encrypt the chunk.
  const uint32_t encrypt_length = ::std::min(length, chunk_size_);
  des_.Encrypt(buffer, encrypt_length, chunk_buffer_);

  // Send the chunk.
  return SendChunk(chunk_buffer_, encrypt_length);
}

}  // namespace common
}  // namespace transfer
}  // namespace hw1
