#ifndef HW1_DES_S_BOX_H_
#define HW1_DES_S_BOX_H_

#include "s_box.h"

namespace hw1 {

// Implements the particular type of 4-bit S-box that we use for DES.
class DesSBox : SBox {
 public:
  // Args:
  //  box: Array representing the box contents in row-major format.
  DesSBox(const uint8_t *box);

  // Performs substitution on an input.
  // Args:
  //  input: The input. Uses the left 4 bits.
  // Returns:
  //  The 2-bit output.
  uint8_t Substitute(uint8_t input);
};

}  // namespace hw1

#endif  // HW1_DES_S_BOX_H_
