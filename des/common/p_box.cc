#include "p_box.h"

#include <string.h>

namespace hw1 {

PBox::PBox(uint16_t length, const uint16_t *bit_order) : block_bits_(length) {
  // Allocate and copy the bit order.
  bit_order_ = new uint16_t[length];
  memcpy(bit_order_, bit_order, length * sizeof(uint16_t));

  // If the block size does not go evenly into 8, there's going to be an extra
  // byte in all our blocks.
  block_bytes_ = length / 8;
  if (length % 8) {
    ++block_bytes_;
  }
}

PBox::~PBox() {
  // Free bit order memory.
  delete[] bit_order_;
}

void PBox::Permute(const uint8_t *block, uint8_t *permutation) {
  // Explicitly zero the permutation.
  memset(permutation, 0, block_bytes_);

  for (uint16_t elem_i = 0; elem_i < block_bytes_; ++elem_i) {
    // Permute every bit in the element.
    for (uint8_t i = 0; i < 8; ++i) {
      // Figure out what to replace it with.
      const uint16_t bit_index = elem_i * 8 + i;
      if (bit_index >= block_bits_) {
        // We don't have any more bits to permute.
        break;
      }

      const uint16_t replace_i = bit_order_[elem_i * 8 + i];
      const uint8_t replace = GetBit(block, replace_i);

      // Do the replacement.
      permutation[elem_i] |= (replace << i);
    }
  }
}

uint8_t PBox::GetBit(const uint8_t *block, uint16_t pos) {
  // Figure out which element it's in.
  const uint16_t index = pos / 8;
  const uint8_t bit_num = pos % 8;
  const uint8_t elem = block[index];

  // Get the bit.
  return (elem >> bit_num) & 0x1;
}

}  // namespace hw1
