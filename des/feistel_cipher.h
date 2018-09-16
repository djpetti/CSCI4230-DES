#ifndef HW1_FEISTEL_CIPHER_H_
#define HW1_FEISTEL_CIPHER_H_

#include <stdint.h>

#include "f_function.h"
#include "common/p_box.h"

namespace hw1 {

// A class that implements the actual cipher for DES.
class FeistelCipher {
 public:
  FeistelCipher();

  // Sets the input to the cipher.
  // Args:
  //  block: The 8-bit input block.
  void SetInput(uint8_t block);
  // Performs an encryption round of the cipher.
  // Args:
  //  key: The 8-bit subkey to use for the round.
  void DoEncRound(uint8_t key);
  // Performs a decryption round of the cipher.
  // Args:
  //  key: The 8-bit subkey to use for the round.
  void DoDecRound(uint8_t key);
  // Gets the current output of the cipher.
  // Returns:
  //  The 8-bit output block.
  uint8_t GetOutput();

 private:
  // The F function to use.
  FFunction f_function_;
  // Performs the initial permutation.
  PBox initial_perm_;
  // Performs the final permutation.
  PBox final_perm_;

  // The current state of the cipher, divided into left and right halves.
  uint8_t left_state_;
  uint8_t right_state_;
};

}  // namespace hw1

#endif  // HW1_FEISTEL_CIPHER_H_
