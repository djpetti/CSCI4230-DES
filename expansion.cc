#include "expansion.h"

#include <assert.h>

namespace hw1 {
namespace {

// Shifts a byte array by a number of bits to the right.
// Args:
//  start: The start of the array to shift. Note that it will write at least
//         <shift> bytes before the start, so that memory should be valid. We
//         assume that we are aligned with the byte boundary to begin with.
//  shift: Number of bits to shift. Should be < 8.
//  length: Number of bytes after start to shift.
void ShiftArray(uint8_t *start, uint8_t shift, uint16_t length) {
  assert(shift < 8);

  for (uint16_t i = 0; i < length; ++i) {
    uint8_t *current = start + i;
    uint8_t *previous = current - 1;

    const uint8_t mask = (*current) << (8 - shift);
    *previous |= mask;
    *current >>= shift;
  }
}

}  // namespace

Expansion::Expansion(const uint16_t *left_order, const uint16_t *right_order,
                     uint16_t length)
    : left_box_(length, left_order),
      right_box_(length, right_order),
      bit_length_(length) {
  // If the block length is not a multiple of 8, we're going to have one extra
  // byte.
  byte_length_ = length / 8;
  if (length % 8) {
    ++byte_length_;
  }
}

void Expansion::Expand(const uint8_t *block, uint8_t *expanded) {
  // Permute both sides.
  left_box_.Permute(block, expanded + byte_length_);
  right_box_.Permute(block, expanded);

  // Shift the start of the second if necessary to make them both aligned.
  const uint16_t shift_by = byte_length_ * 8 - bit_length_;
  ShiftArray(expanded + byte_length_, shift_by, byte_length_);
}

}  // namespace hw1
