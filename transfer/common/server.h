#ifndef HW1_TRANSFER_COMMON_SERVER_H_
#define HW1_TRANSFER_COMMON_SERVER_H_

#include <stdint.h>

#include "des/des.h"

namespace hw1 {
namespace transfer {
namespace common {

// A base class for implementing a secure, simple socket server. It can handle a
// maximum of one client connection at a time.
class Server {
 public:
  // Args:
  //  key: The key to use for encrypting and decrypting data. Passed as a
  //       2-length byte array.
  //  chunk_size: Maximum-size chunk of data we will be receiving at once.
  Server(const uint8_t *key, uint32_t chunk_size);
  ~Server();

  // Sets the key to use for encrypting and decrypting data.
  // Args:
  //  key: The key to set, as a 2-length byte array.
  void SetKey(const uint8_t *key);

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
  uint32_t ReceiveChunk(char **buffer, uint32_t length);
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
  uint32_t ReceiveAndDecryptChunk(char **buffer, uint32_t length);

  // Sends a single chunk to the client.
  // Args:
  //  buffer: The buffer containing the message to be sent.
  //  length: The length of the message in bytes.
  // Returns:
  //  The number of bytes that it sent.
  uint32_t SendChunk(const char *buffer, uint32_t length);
  // Encrypts and sends a single chunk to the client.
  // Args:
  //  buffer: The buffer containing the message to be sent.
  //  length: The length of the message in bytes.
  // Returns:
  //  The number of bytes that were sent.
  uint32_t EncryptAndSendChunk(const char *buffer, uint32_t length);

  // Closes the client socket and indicates that no client is connected.
  void CleanUp();

  // Buffer for storing encrypted data being sent or received.
  char *chunk_buffer_;
  // Second chunk buffer to handle decrypted chunks.
  char *plain_chunk_buffer_;

 private:
  // Maximum size of chunks that we will be receiving.
  uint32_t chunk_size_;

  // The socket the server is using.
  int socket_;
  // The client socket.
  int client_sock_ = -1;

  // DES implementation to use for encryption and decryption.
  Des des_;
};

}  // namespace common
}  // namespace transfer
}  // namespace hw1

#endif  // HW1_TRANSFER_COMMON_SERVER_H_
