#include "f_function.h"

namespace hw1 {
namespace {

// Bit orders for expansion.
//const uint16_t kExpLeftBitOrder[] = {3, 0, 1, 2};
//const uint16_t kExpRightBitOrder[] = {1, 2, 3, 0};
const uint16_t kExpLeftBitOrder[] = {0, 1, 2, 3};
const uint16_t kExpRightBitOrder[] = {0, 1, 2, 3};

// Bit order for final permutation.
const uint16_t kFinalBitOrder[] = {1, 3, 2, 0};

// S box contents.
const uint8_t kLeftSBox[] = {1, 0, 3, 2, 3, 2, 1, 0, 0, 2, 1, 3, 3, 1, 3, 2};
const uint8_t kRightSBox[] = {0, 1, 2, 3, 2, 0, 1, 3, 3, 0, 1, 0, 2, 1, 0, 3};

}  // namespace

FFunction::FFunction()
    : expansion_(kExpLeftBitOrder, kExpRightBitOrder, 4),
      left_sbox_(kLeftSBox),
      right_sbox_(kRightSBox),
      final_perm_(4, kFinalBitOrder) {}

uint8_t FFunction::Call(uint8_t input, uint8_t key) {
  // Expand the input.
  uint8_t expanded[2];
  expansion_.Expand(&input, expanded);

  // XOR the key.
  expanded[0] ^= key;

  // Substitute left and right.
  uint8_t right = expanded[0] & 0x0F;
  uint8_t left = expanded[0] >> 4;

  left = left_sbox_.Substitute(left);
  right = right_sbox_.Substitute(right);

  // Join the 2-bit outputs.
  const uint8_t joined = (left << 2) | right;

  // Do the final permutation.
  uint8_t output;
  final_perm_.Permute(&joined, &output);

  return output;
}

}  // namespace hw1
