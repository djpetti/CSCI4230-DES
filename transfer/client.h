#ifndef HW1_TRANSFER_CLIENT_H_
#define HW1_TRANSFER_CLIENT_H_

#include <stdint.h>
#include <stdio.h>

#include "common/client.h"

namespace hw1 {
namespace transfer {

// Represents a client that sends encrypted files.
class Client : public common::Client {
 public:
  // Args:
  //  key: The 10-bit encryption key to use for file transfers, as a 2-length
  //       byte array.
  Client(const uint8_t *key);

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
};

}  // namespace transfer
}  // namespace hw1

#endif  // HW1_TRANSFER_CLIENT_H_
