#ifndef HW1_TRANSFER_COMMON_CLIENT_H_
#define HW1_TRANSFER_COMMON_CLIENT_H_

#include <stdint.h>
#include <stdio.h>

#include "secure_node.h"

namespace hw1 {
namespace transfer {
namespace common {

// Represents a client that sends encrypted data.
class Client : public SecureNode {
 public:
  // Args:
  //  key: The key to use for encrypting and decrypting data. Passed as a
  //       2-length byte array.
  //  chunk_size: Maximum-size chunk of data we will be sending or receiving at
  //              once.
  Client(const uint8_t *key, uint32_t chunk_size);
  virtual ~Client();

  // Connects the client to a server.
  // Args:
  //  server: The server address to connect to.
  //  port: The server port to connect to.
  // Returns:
  //  True if the connection succeeded, false otherwise.
  bool Connect(const char *server, uint16_t port);
  // Closes the existing connection.
  void Close();

 protected:
  uint32_t ReceiveChunk(char **buffer);
  uint32_t ReceiveChunk(char **buffer, uint32_t length);
  uint32_t ReceiveAndDecryptChunk(char **buffer);
  uint32_t ReceiveAndDecryptChunk(char **buffer, uint32_t length);

  uint32_t SendChunk(const char *buffer, uint32_t length);
  uint32_t EncryptAndSendChunk(const char *buffer, uint32_t length);

 private:
  // The socket we are connected on.
  int socket_;
};

}  // namespace common
}  // namespace transfer
}  // namespace hw1

#endif  // HW1_TRANSFER_COMMON_CLIENT_H_
