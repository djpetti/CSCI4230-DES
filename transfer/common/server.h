#ifndef HW1_TRANSFER_COMMON_SERVER_H_
#define HW1_TRANSFER_COMMON_SERVER_H_

#include <stdint.h>

#include "secure_node.h"

namespace hw1 {
namespace transfer {
namespace common {

// A base class for implementing a secure, simple socket server. It can handle a
// maximum of one client connection at a time.
class Server : public SecureNode {
 public:
  // Args:
  //  key: The key to use for encrypting and decrypting data. Passed as a
  //       2-length byte array.
  //  chunk_size: Maximum-size chunk of data we will be sending or receiving at
  //              once.
  Server(const uint8_t *key, uint32_t chunk_size);
  virtual ~Server();

  // Starts the server listening. This should be called before handling
  // connections.
  // Args:
  //  port: The port to listen on.
  // Returns:
  //  True if listening was successful, false otherwise.
  bool Listen(uint16_t port);
  // Waits for the next connection.
  // Returns:
  //  True if this process was successful, false if it encountered an error.
  bool WaitForConnection();

 protected:
  // Returns:
  //  True if a client is currently connected, false otherwise.
  bool ClientConnected();

  uint32_t ReceiveChunk(char **buffer);
  uint32_t ReceiveChunk(char **buffer, uint32_t length);
  uint32_t ReceiveAndDecryptChunk(char **buffer);
  uint32_t ReceiveAndDecryptChunk(char **buffer, uint32_t length);

  uint32_t EncryptAndSendChunk(const char *buffer, uint32_t length);
  uint32_t SendChunk(const char *buffer, uint32_t length);

  // Closes the client socket and indicates that no client is connected.
  void CleanUp();

 private:
  // The socket the server is using.
  int socket_;
  // The client socket.
  int client_sock_ = -1;
};

}  // namespace common
}  // namespace transfer
}  // namespace hw1

#endif  // HW1_TRANSFER_COMMON_SERVER_H_
