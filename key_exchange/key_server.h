#ifndef HW1_KEY_EXCHANGE_KEY_SERVER_H_
#define HW1_KEY_EXCHANGE_KEY_SERVER_H_

#include <unordered_map>

#include "des/des.h"
#include "transfer/common/server.h"
#include "diffie_hellman.h"
#include "nonce_generator.h"

namespace hw1 {
namespace key_exchange {

// Internally, we use this to reprent keys in the map.
struct Key {
  uint8_t Key[2];
};

// Server that is responsible for distributing keys to clients.
class KeyServer : public transfer::common::Server {
 public:
  KeyServer();

  // Waits for a client to connect, and handles all requests from that client,
  // until the client disconnects.
  void HandleConnection();

 protected:
  // Disconnects the client nicely.
  void CleanUp();

 private:
  // Handles a single transaction from a client.
  // Returns:
  //  True if the transaction completed, false if there was a failure before
  //  completion.
  bool HandleTransaction();
  // Reads and decodes the first message from the client.
  // Args:
  //  id_a: Set to the ID of the first client.
  //  id_b: Set to the ID of the second client.
  //  nonce: Set to the nonce value.
  // Returns:
  //  True if reading was successful, false if it failed to read or there was a
  //  problem with the message.
  bool GetFirstMessage(uint8_t *id_a, uint8_t *id_b, uint32_t *nonce);
  // Generates a new session key and sends it to the requester.
  // Args:
  //  id_a: The ID of the first client.
  //  id_b: The ID of the second client.
  //  nonce: The nonce value.
  // Returns:
  //  True if generation and sending was successful, false if either failed.
  bool GenerateAndSendKey(uint8_t id_a, uint8_t id_b, uint32_t nonce);
  // Generates a 10-bit key.
  // Args:
  //  key: 2-length byte array where the key will be written.
  // Returns:
  //  True upon successful generation, false otherwise.
  bool GenerateKey(uint8_t *key);
  // Performs the initial Diffie-Hellman key exchange with a node.
  // Args:
  //  node_id: The node that we are setting a key for.
  // Returns:
  //  True if the exchange succeeded, false otherwise.
  bool DoKeyExchange(uint8_t node_id);

  // Maps client IDs to their corresponding keys.
  ::std::unordered_map<uint8_t, Key> client_keys_;
  // Maps client IDs to the last nonce value we received from that client.
  ::std::unordered_map<uint8_t, uint32_t> nonces_;
  // DES instance we use for encryption and decryption.
  Des des_;
  // Nonce generator.
  NonceGenerator nonce_generator_;
  // Used for performing the initial Diffie-Hellman key exchange.
  DiffieHellman key_gen_;
};

}  // namespace key_exchange
}  // namespace hw1

#endif  // HW1_KEY_EXCHANGE_KEY_SERVER_H_
