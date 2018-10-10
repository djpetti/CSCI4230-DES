#ifndef HW1_KEY_EXCHANGE_KEY_MANAGER_H_
#define HW1_KEY_EXCHANGE_KEY_MANAGER_H_

#include <stdint.h>

#include "nonce_generator.h"
#include "transfer/common/client.h"
#include "diffie_hellman.h"

namespace hw1 {
namespace key_exchange {

// Encapsulates all the functionality necessary for exchanging keys with the
// KDC.
class KeyManager : public transfer::common::Client {
 public:
  // Args:
  //  kdc_address: The address of the KDC.
  //  port: The port to connect to the KDC on.
  //  id: The numerical ID of this node.
  KeyManager(const char *kdc_address, const uint16_t port, uint8_t id);

  // Gets the master key to use for communicating with the KDC. It will
  // automatically perform the key exchange if this key is not set.
  // Args:
  //  key: A 2-length byte array which will be set to the 10-bit master key.
  // Returns:
  //  True if getting the key succeeded, false otherwise.
  bool GetMasterKey(uint8_t *key);

 private:
  // Ensures that the key exchange has been properly performed. If it hasn't it
  // connects to the KDC and does so.
  // Returns:
  //  True if exchange succeeded, false otherwise.
  bool EnsureKeyExchanged();
  // Sends the initial key request to the server.
  // Returns:
  //  True if sending succeeded, false otherwise.
  bool SendKeyRequest();
  // Performs the initial key exchange with the KDC.
  // Returns:
  //  True if the key exchange succeeded, false otherwise.
  bool DoKeyExchange();

  // Saves the address of the key server.
  const char *kdc_address_;
  // Saves the port to communicate with the key server on.
  uint16_t kdc_port_;
  // The master key to use for communicating with the KDC, as a 2-length byte
  // array.
  uint8_t kdc_key_[2];

  // The numerical ID of this node.
  uint8_t id_;

  // Whether the key exchange has completed successfully.
  bool set_key_ = false;

  // Diffie-Hellman implementation that we use for generating keys.
  DiffieHellman key_gen_;
  // Use for generating nonces.
  NonceGenerator nonce_generator_;
};

}  // namespace key_exchange
}  // namespace hw1

#endif  // HW1_KEY_EXCHANGE_KEY_MANAGER_H_
