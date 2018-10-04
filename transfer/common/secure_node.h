#ifndef HW1_TRANSFER_COMMON_SECURE_NODE_H_
#define HW1_TRANSFER_COMMON_SECURE_NODE_H_

#include <stdint.h>

#include "des/des.h"

namespace hw1 {
namespace transfer {
namespace common {

// Defines some basic functionality for secure clients and servers.
class SecureNode {
 public:
  // Args:
  //  key: The key to use for encrypting and decrypting data. Passed as a
  //       2-length byte array.
  //  chunk_size: Maximum-size chunk of data we will be sending or receiving at
  //              once.
  SecureNode(const uint8_t *key, uint32_t chunk_size);
  virtual ~SecureNode();

  // Sets the key to use for encrypting and decrypting data.
  // Args:
  //  key: The key to set, as a 2-length byte array.
  void SetKey(const uint8_t *key);

 protected:
  // Receives a single chunk from the client.
  // Args:
  //  buffer: Will be set to the address of the buffer that the data is in.
  // Returns:
  //  The number of bytes that were received.
  uint32_t ReceiveChunk(char **buffer);
  // Same as above, but allows user to specify an optional length parameter.
  // This is the maximum amount of data we will try to receive.
  // Args:
  //  length: The maximum number of bytes to receive.
  virtual uint32_t ReceiveChunk(char **buffer, uint32_t length) = 0;
  // Receives and decrypts a single chunk from the client.
  // Args:
  //  buffer: Will be set to the address of the buffer that the data is in.
  // Returns:
  //  The number of bytes that were received.
  uint32_t ReceiveAndDecryptChunk(char **buffer);
  // Same as above, but allows user to specify an optional length parameter.
  // This is the maximum amount of data we will try to receive.
  // Args:
  //  length: The maximum number of bytes to receive.
  virtual uint32_t ReceiveAndDecryptChunk(char **buffer, uint32_t length) = 0;

  // Sends a single chunk to the client.
  // Args:
  //  buffer: The buffer containing the message to be sent.
  //  length: The length of the message in bytes.
  // Returns:
  //  The number of bytes that it sent.
  virtual uint32_t SendChunk(const char *buffer, uint32_t length) = 0;
  // Encrypts and sends a single chunk to the client.
  // Args:
  //  buffer: The buffer containing the message to be sent.
  //  length: The length of the message in bytes.
  // Returns:
  //  The number of bytes that were sent.
  uint32_t EncryptAndSendChunk(const char *buffer, uint32_t length);

  // Maximum size of chunks that we will be receiving.
  uint32_t chunk_size_;

  // Buffer for storing encrypted data being sent or received.
  char *chunk_buffer_;
  // Second chunk buffer to handle decrypted chunks.
  char *plain_chunk_buffer_;

  // DES implementation to use for encryption and decryption.
  Des des_;
};

}  // namespace common
}  // namespace transfer
}  // namespace hw1

#endif  // HW1_TRANSFER_COMMON_SECURE_NODE_H_
