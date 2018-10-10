#ifndef HW1_TRANSFER_FILE_SERVER_H_
#define HW1_TRANSFER_FILE_SERVER_H_

#include <stdint.h>

#include "common/server.h"

namespace hw1 {
namespace transfer {

// A class for implementing a secure file transfer server.
class Server : public common::Server {
 public:
  // Args:
  //  key: The key to use for decrypting files. Passed as a 2-length byte array.
  Server(const uint8_t *key);

  // Waits for a client to connect, and handles all transfers from that client.
  bool HandleConnection();

 private:
  // Handles a single message from the client.
  // Returns:
  //  True if the message was successfully handled, false otherwise. Note that
  //  if the client disconnected, that qualifies as success.
  bool HandleMessage();
  // Handles receiving a file.
  // Args:
  //  filename: The name of the file to write.
  //  file_size: The size of the file.
  // Returns:
  //  True if the file was successfully received, false otherwise.
  bool ReceiveFile(const char *filename, uint32_t file_size);
};

}  // namespace transfer
}  // namespace hw1

#endif  // HW1_TRANSFER_FILE_SERVER_H_
