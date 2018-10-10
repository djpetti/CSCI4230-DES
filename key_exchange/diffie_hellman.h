#ifndef HW1_KEY_EXCHANGE_DIFFIE_HELLMAN_H_
#define HW1_KEY_EXCHANGE_DIFFIE_HELLMAN_H_

#include <stdint.h>

namespace hw1 {
namespace key_exchange {

// Implements the computational Diffie-Hellman key exchange protocol.
class DiffieHellman {
 public:
  // Creates a random private key.
  // Returns:
  //  The random private key that it created, or 0 upon failure.
  uint64_t GeneratePrivateKey();
  // Computes the value of the public key given the private key.
  // Args:
  //  key: The private key.
  // Returns:
  //  The public key.
  uint64_t ComputePublicKey(uint64_t private_key);
  // Computes the value of the session key given the other user's public key
  // and the private key.
  // Args:
  //  pub_key: The other user's public key.
  //  private_key: Our private key.
  // Returns:
  //  The session key.
  uint64_t ComputeSessionKey(uint64_t pub_key, uint64_t private_key);
};

}  // namespace key_exchange
}  // namespace hw1

#endif  // HW1_KEY_EXCHANGE_DIFFIE_HELLMAN_H_
