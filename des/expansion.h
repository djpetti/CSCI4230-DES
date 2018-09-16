#ifndef HW1_EXPANSION_H_
#define HW1_EXPANSION_H_

#include "common/p_box.h"

namespace hw1 {

// A class that expands a bit string by permuting it twice and concatenating the
// permutations.
class Expansion {
 public:
  // Args:
  //  left_order: The bit order to use for the left side of the expansion.
  //  right_order: The bit order to use for the right side of the expansion.
  //  length: The input block size, in bits.
  Expansion(const uint16_t *left_order, const uint16_t *right_order,
            uint16_t length);

  // Expands an input block.
  // Args:
  //  block: The block to expand.
  //  expanded: Where to right the output block. Should be at least twice the
  //            size of block.
  void Expand(const uint8_t *block, uint8_t *expanded);

 private:
  // The internal P boxes to use for the permutations.
  PBox left_box_;
  PBox right_box_;
  // The length of the blocks in bytes.
  uint16_t byte_length_;
  // The length of the blocks in bits.
  uint16_t bit_length_;
};

}  // namespace hw1

#endif  // HW1_EXPANSION_H_
