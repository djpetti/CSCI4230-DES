#include "feistel_cipher.h"

#include <algorithm>

namespace hw1 {
namespace {

// The bit order of the initial permutation.
const uint16_t kInitialBitOrder[] = {1, 5, 2, 0, 3, 7, 4, 6};
// The bit order of the final permutation.
const uint16_t kFinalBitOrder[] = {3, 0, 2, 4, 6, 1, 7, 5};

}  // namespace

FeistelCipher::FeistelCipher()
    : initial_perm_(8, kInitialBitOrder), final_perm_(8, kFinalBitOrder) {}

void FeistelCipher::SetInput(uint8_t block) {
  // Perform initial permutation.
  uint8_t state;
  initial_perm_.Permute(&block, &state);

  // Split the state.
  right_state_ = state & 0x0F;
  left_state_ = state >> 4;
}

void FeistelCipher::DoRound(uint8_t key) {
  // Run the F function.
  left_state_ = f_function_.Call(left_state_, key);

  // Combine the sides.
  right_state_ ^= left_state_;

  // Swap for next round.
  ::std::swap(left_state_, right_state_);
}

uint8_t FeistelCipher::GetOutput() {
  // Combine the sides.
  const uint8_t state = (left_state_ << 4) | right_state_;

  // Perform the final permutation.
  uint8_t output;
  final_perm_.Permute(&state, &output);

  return output;
}

}  // namespace hw1
