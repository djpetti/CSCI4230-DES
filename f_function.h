#ifndef HW1_F_FUNCTION_H_
#define HW1_F_FUNCTION_H_

#include <stdint.h>

#include "des_s_box.h"
#include "expansion.h"
#include "p_box.h"

namespace hw1 {

// Represents the F function to use.
class FFunction {
 public:
  FFunction();

  // Call the function on an input, and get the next output.
  // Args:
  //  input: A 4-bit input to the funtion. Stored in the lower 4 bits.
  //  key: The 8-bit subkey.
  // Returns:
  //  The 4-bit output from the function.
  uint8_t Call(uint8_t input, uint8_t key);

 private:
  // Initial expansion.
  Expansion expansion_;
  // Left and right S-boxes.
  DesSBox left_sbox_;
  DesSBox right_sbox_;
  // Final permutation.
  PBox final_perm_;
};

}  // namespace hw1

#endif  // HW1_F_FUNCTION_H_
