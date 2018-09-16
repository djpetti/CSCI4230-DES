#ifndef HW1_TRANSFER_SERVER_H_
#define HW1_TRANSFER_SERVER_H_

#include <stdint.h>

#include "des/des.h"

namespace hw1 {
namespace transfer {

// A class for implementing a secure file transfer server.
class Server {
 public:
  // Args:
  //  key: The key to use for decrypting files. Passed as a 2-length byte array.
  Server(const uint8_t *key);
  ~Server();

  // Starts the server listening. This should be called before handling
  // connections.
  // Args:
  //  port: The port to listen on.
  // Returns:
  //  True if listening was successful, false otherwise.
  bool Listen(uint16_t port);
  // Handles the next connection. It waits for someone to connect, and then it
  // waits for them to either disconnect, or to receive a file from them.
  // Returns:
  //  True if this process was successful, false if it encountered an error.
  bool HandleConnection();

 private:
  // Returns:
  //  True if a client is currently connected, false otherwise.
  bool ClientConnected();
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
  // Cleans up a client connection.
  void CleanUp();

  // The socket the server is using.
  int socket_;
  // The client socket.
  int client_sock_ = -1;

  // Buffers for reading the message header and file chunks.
  char *header_buffer_;
  char *chunk_buffer_;
  // Second chunk buffer to handle decrypted chunks.
  char *plain_chunk_buffer_;

  // DES implementation to use for decryption.
  Des decryptor_;
};

}  // namespace transfer
}  // namespace hw1

#endif  // HW1_TRANSFER_SERVER_H_
