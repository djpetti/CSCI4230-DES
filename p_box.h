#ifndef HW1_PBOX_H_
#define HW1_PBOX_H_

#include <stdint.h>

namespace hw1 {

// A class that implements permutations.
class PBox {
 public:
  // Args:
  //  length: The block length, in bits.
  //  bit_order: The bit ordering to use for the permutation. Should be an array
  //             that is the same length as the block size.
  PBox(uint16_t length, const uint16_t *bit_order);
  ~PBox();

  // Permutes a new input.
  // Args:
  //  block: The input to permute. If the block length is not a multiple of 8,
  //         the bits at the end of the block will be ignored.
  //  permutation: The output permuted block.
  void Permute(const uint8_t *block, uint8_t *permutation);

 private:
  // Gets the value of a bit at a particular position in block.
  // Args:
  //  block: The block to get the bit from.
  //  pos: The position of the bit to get.
  uint8_t GetBit(const uint8_t *block, uint16_t pos);

  // Will store the bit order constant.
  uint16_t *bit_order_;
  // Block size in bytes.
  uint16_t block_bytes_;
  // Block size in bits.
  uint16_t block_bits_;
};

}  // namespace hw1

#endif  // HW1_PBOX_H_
