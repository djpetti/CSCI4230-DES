#ifndef HW1_SEMANTIC_SECURITY_BLUM_GOLDWASSER_H_
#define HW1_SEMANTIC_SECURITY_BLUM_GOLDWASSER_H_

#include <stdint.h>

#include "blum_goldwasser_public.h"

namespace hw1 {
namespace semantic_security {

// Implements Blum-Goldwasser algorithm.
class BlumGoldwasser : public BlumGoldwasserPublic {
 public:
  // Args:
  //  p: A Blum integer.
  //  q: Another Blum integer.
  //  a: Private key integer 1.
  //  b: Private key integer 2.
  //  x_0: Seed in QRn.
  BlumGoldwasser(uint64_t p, uint64_t q, int64_t a, int64_t b, uint64_t x_0);

  // Decrypts a message.
  // Args:
  //  cipher: The ciphertext.
  //  length: The length of the ciphertext, in bytes.
  //  last_x: The x_t+1 output from the encryption.
  //  plain: The message, as a uint8_t array.
  void Decrypt(const uint8_t *cipher, uint32_t length, uint64_t last_x,
               uint8_t *plain);

 private:
  // Reconstructs the initial state of the RNG based on the last state included
  // in the encrypted message.
  // Args:
  //  last_x: The last random number from the encryption stage.
  //  length: The length of the message, in bytes.
  void ReconstructRngState(uint64_t last_x, uint32_t length);

  // Internal cryptosystem variables.
  uint64_t p_;
  uint64_t q_;
  int64_t a_;
  int64_t b_;
};

}  // namespace semantic_security
}  // namespace semantic_security

#endif  // HW1_SEMANTIC_SECURITY_BLUM_GOLDWASSER_H_
