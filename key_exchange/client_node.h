#ifndef HW1_KEY_EXCHANGE_CLIENT_NODE_H_
#define HW1_KEY_EXCHANGE_CLIENT_NODE_H_

#include <stdint.h>

#include <unordered_map>

#include "transfer/common/client.h"
#include "key_manager.h"
#include "nonce_generator.h"

namespace hw1 {
namespace key_exchange {

// Implements a client node in our key exchange system. The way this works is as
// follows: The client connects to the KDC, requests a key, and sends that key
// along to a server node. Then, the client node and server node can communicate
// securely.
class ClientNode : public transfer::common::Client {
 public:
  // Args:
  //  kdc_address: The address of the KDC.
  //  port: The port to connect to the KDC on.
  //  id: The numerical ID of this node.
  ClientNode(const char *kdc_address, const uint16_t port, uint8_t id);

  // Associates an address with the particular ID of that node.
  // Args:
  //  address: The address of the node.
  //  id: The numerical ID of the node.
  void AddNodeWithId(const char *address, uint8_t id);
  // Connects the client to a server. A new key will automatically be requested
  // from the KDC, and a handshake will be performed with the server.
  // Args:
  //  server: The server address to connect to.
  //  port: The server port to connect to.
  // Returns:
  //  True if the connection succeeded, false otherwise.
  bool Connect(const char *server, uint16_t port);

  // Sends an encrypted message to the server.
  // Args:
  //  message: The message to send.
  // Returns:
  //  True if it successfully sent the message, false otherwise.
  bool SendMessage(const char *message);

 private:
  // Requests a session key from the key server.
  // Args:
  //  address: The address of the node we are connecting to.
  // Returns:
  //  True if the request succeeded, false otherwise.
  bool RequestSessionKey(const char *address);
  // Receives the session key message after one has been requested.
  // Args:
  //  session_key: Set to the received session key, as a 2-length byte array.
  //  node_envelope: Set to the encrypted envelope to send to the other node. It
  //                 should be able to hold at least 7 bytes.
  // Returns:
  //  True if receiving the message succeeded, false otherwise.
  bool ReceiveSessionKey(uint8_t *session_key, char *node_envelope);

  // Saves the address of the key server.
  const char *kdc_address_;
  // Saves the port to communicate with the key server on.
  uint16_t kdc_port_;

  // The numerical ID of this node.
  uint8_t id_;
  // Associates node addresses with numerical IDs.
  ::std::unordered_map<const char *, uint8_t> node_ids_;

  // Generator to use for producing nonces.
  NonceGenerator nonce_generator_;
  // Used internally for exchanging master keys with the KDC.
  KeyManager key_manager_;
};

}  // namespace key_exchange
}  // namespace hw1

#endif  // HW1_KEY_EXCHANGE_CLIENT_NODE_H_
