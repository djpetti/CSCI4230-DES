#include "key_gen.h"

namespace hw1 {
namespace {

// Initial permutation bit order.
const uint16_t kP10BitOrder[] = {2, 4, 1, 6, 3, 9, 0, 8, 7, 5};
// Final permutation bit order.
const uint16_t kP8BitOrder[] = {5, 2, 6, 3, 7, 4, 9, 8};

}  // namespace

KeyGen::KeyGen(const uint8_t *start_key)
    : initial_perm_(10, kP10BitOrder), final_perm_(8, kP8BitOrder) {
  // Perform initial permutation.
  initial_perm_.Permute(start_key, state_);
}

uint8_t KeyGen::GenNext() {
  // Extract left and right 5 bits.
  uint8_t left_state = state_[0] & (0xFF >> 3);
  uint8_t right_state = (state_[0] >> 5) | ((state_[1] & 0x3) << 3);

  // When we left-shift, we want to roll over, so we don't get progressively
  // more zeros as time goes on.
  left_state = (left_state << 1) | (left_state >> 7);
  right_state = (right_state << 1) | (right_state >> 7);

  // Write the new combined state.
  state_[0] = left_state | (right_state << 5);
  state_[1] = right_state >> 5;

  // Permute to generate the next subkey.
  uint8_t subkey;
  final_perm_.Permute(state_, &subkey);

  return subkey;
}

}  // namespace hw1
