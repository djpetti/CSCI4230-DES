#ifndef HW1_KEY_GEN_H_
#define HW1_KEY_GEN_H_

#include <stdint.h>

#include "common/p_box.h"

namespace hw1 {

// Generates subkeys for each round.
class KeyGen {
 public:
  // Args:
  //  start_key: The initial 10-bit key, in an array of two uint8_ts.
  KeyGen(const uint8_t *start_key);

  // Resets the state of the key generator with a new starting key.
  // Args:
  //  key: The new starting key, as a 2-length byte array.
  void Reset(const uint8_t *key);

  // Generates the next subkey.
  // Returns:
  //  The next 8-bit subkey.
  uint8_t GenNext();

 private:
  // Current ten-bit state. This is implemented as an array of 2 uint8_ts.
  uint8_t state_[2];

  // Initial permutation.
  PBox initial_perm_;
  // Final permutation.
  PBox final_perm_;
};

}  // namespace hw1

#endif  // HW1_KEY_GEN_H_
