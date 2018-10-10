#ifndef HW1_KEY_EXCHANGE_SERVER_NODE_H_
#define HW1_KEY_EXCHANGE_SERVER_NODE_H_

#include <stdint.h>

#include "transfer/common/server.h"
#include "key_manager.h"
#include "nonce_generator.h"

namespace hw1 {
namespace key_exchange {

// Implementes a server node in our key exchange system. The way this works is
// as follows: The server sits there and waits for a client to connect and send
// it the NS handshake message. Once it receives the message, it can securely
// communicate with that client.
class ServerNode : public transfer::common::Server {
 public:
  // Args:
  //  kdc_address: The address of the KDC.
  //  kdc_port: The port that we communicate with the KDC on.
  //  id: The numerical ID of this node.
  ServerNode(const char *kdc_address, uint16_t kdc_port, uint8_t id);

  // Waits for a new connection to the server, and handles that connection until
  // the client disconnects.
  // Returns:
  //  True if the connection was properly set up, false if it failed to connect
  //  or the handshake failed.
  bool HandleConnection();

 private:
  // Performs the handshake that we need to make with the client before we can
  // start communicating securely.
  // Returns:
  //  True if the handshake was successfully performed, false otherwise.
  bool DoHandshake();
  // Handles a single transaction from the client.
  // Returns:
  //  True if the transaction was successful, false if it failed or the client
  //  disconnected.
  bool HandleTransaction();

  // The numerical ID of this node.
  uint8_t id_;

  // Use this for generating nonces.
  NonceGenerator nonce_generator_;
  // Manages key exchange between this node and the KDC.
  KeyManager key_manager_;
};

}  // namespace key_exchange
}  // namespace hw1

#endif  // HW1_KEY_EXCHANGE_SERVER_NODE_H_
