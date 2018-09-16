#ifndef HW1_TRANSFER_CLIENT_H_
#define HW1_TRANSFER_CLIENT_H_

#include <stdint.h>
#include <stdio.h>

#include "des/des.h"

namespace hw1 {
namespace transfer {

// Represents a client that sends encrypted data.
class Client {
 public:
  // Args:
  //  key: The 10-bit encryption key to use for file transfers, as a 2-length
  //       byte array.
  Client(const uint8_t *key);
  ~Client();

  // Connects the client to a server.
  // Args:
  //  server: The server address to connect to.
  //  port: The server port to connect to.
  // Returns:
  //  True if the connection succeeded, false otherwise.
  bool Connect(const char *server, uint16_t port);

  // Sends a new file to the server.
  // Args:
  //  filename: The name of the file to send.
  // Returns:
  //  True if it successfully sent the file, false otherwise.
  bool SendFile(const char *filename);

 private:
  // Constructs and sends the header for a file transfer.
  // Args:
  //  filename: The name of the file to send the header for.
  //  file: The actual file.
  // Returns:
  //  The size of the file if it sent the header successfully, or 0 if it
  //  didn't.
  uint32_t SendHeader(const char *filename, FILE *file);

  // The socket we are connected on.
  int socket_;

  // Buffer to use internally for storing the message header.
  char *header_buffer_;
  // Buffer to use internally for storing chunks of file data.
  char *chunk_buffer_;
  // Second buffer for storing encrypted chunks.
  char *cipher_chunk_buffer_;

  // DES implemetation to use for encrypting file data.
  Des encryptor_;
};

}  // namespace transfer
}  // namespace hw1

#endif  // HW1_TRANSFER_CLIENT_H_
