#include "diffie_hellman.h"

#include <stdio.h>
#include <sys/random.h>

namespace hw1 {
namespace key_exchange {
namespace {

// The prime modulo to use for computation.
const uint64_t kModulo = 2698727;
// The primitive root to use. Note that I'm not actually sure that this is the
// primitive root, however, since kModulo is a "safe" prime, the probability
// that it isn't is pretty low.
const uint64_t kGenerator = 3;

// Implementation of square and multiply algorithm for module integer
// exponentiation.
// Args:
//  base: The base.
//  exp: The exponent.
//  mod: The modulo.
// Returns:
//  base^exp % mod
uint64_t Power(uint64_t base, uint64_t exp, uint64_t mod) {
  uint64_t pow = 1;
  for (uint8_t i = 0; i < 64; ++i) {
    const uint8_t bit = 63 - i;

    pow *= pow;
    pow %= mod;
    if (exp & ((uint64_t)1 << bit)) {
      pow *= base;
      pow %= mod;
    }
  }

  return pow;
}

}  // namespace

uint64_t DiffieHellman::ComputePublicKey(uint64_t private_key) {
  // Note that the user is responsible for avoiding integer overflows.
  return Power(kGenerator, private_key, kModulo);
}

uint64_t DiffieHellman::ComputeSessionKey(uint64_t pub_key,
                                          uint64_t private_key) {
  // Note that the user is responsible for avoiding integer overflows.
  return Power(pub_key, private_key, kModulo);
}

uint64_t DiffieHellman::GeneratePrivateKey() {
  // Again, use Linux's secure RNG to generate this.
  uint64_t random;
  const uint32_t got_bytes = getrandom((uint8_t *)&random, 8, 0);
  if (got_bytes != 8) {
    // Failed to generate random number.
    perror("ERROR");
    return 0;
  }

  // Make it in range.
  return random % kModulo + 1;
}

}  // namespace key_exchange
}  // namespace hw1
