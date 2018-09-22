#include "diff_analysis.h"

#include <stdint.h>
#include <stdio.h>

namespace hw1 {
namespace analysis {
namespace {

// The bit order for the initial permutation permutation.
const uint16_t kInitialBitOrder[] = {1, 5, 2, 0, 3, 7, 4, 6};
// The bit order of the final permutation.
const uint16_t kFinalBitOrder[] = {3, 0, 2, 4, 6, 1, 7, 5};

// Bit order for left side of expansion.
const uint16_t kExpLeftBitOrder[] = {3, 0, 1, 2};
// Bit order for inverse of left side of expansion.
const uint16_t kInvExpLeftBitOrder[] = {1, 2, 3, 0};

// Bit order for inverse round function permutation.
const uint16_t kInvRoundBitOrder[] = {3, 0, 2, 1};

}  // namespace

Analyzer::Analyzer(uint8_t input_xor, uint8_t output_xor, const uint8_t *key)
    : input_xor_(input_xor),
      output_xor_(output_xor),
      // Use only the first round for DES, since we only want that subkey.
      des_(key, 1),
      init_perm_(8, kInitialBitOrder),
      final_perm_(8, kFinalBitOrder),
      round_left_exp_(4, kExpLeftBitOrder),
      inv_round_left_exp_(4, kInvExpLeftBitOrder),
      inv_round_perm_(4, kInvRoundBitOrder) {}

void Analyzer::TryInput(uint8_t plaintext) {
  // To compute the pair, we have to deal with the effects of the initial
  // permutation.
  uint8_t plain_perm;
  init_perm_.Permute(&plaintext, &plain_perm);
  // Compute the left side of the expansion in the round function, since this is
  // what goes into S0.
  uint8_t s0_input;
  round_left_exp_.Permute(&plain_perm, &s0_input);

  // Compute the XOR pair.
  const uint8_t s0_input_pair = s0_input ^ input_xor_;
  printf("Pre-key S-box input pair: 0x%X, 0x%X\n", s0_input, s0_input_pair);

  // Transform it back to the plaintext input.
  uint8_t p_pair, p_pair_perm;
  inv_round_left_exp_.Permute(&s0_input_pair, &p_pair_perm);
  // The final permutation is the inverse of the initial one.
  final_perm_.Permute(&p_pair_perm, &p_pair);
  printf("Input pair: 0x%X, 0x%X\n", plaintext, p_pair);

  // Compute the ciphertexts.
  uint8_t cipher1, cipher2;
  des_.Encrypt((const char *)&plaintext, 1, (char *)&cipher1);
  des_.Encrypt((const char *)&p_pair, 1, (char *)&cipher2);
  printf("Raw Ciphertexts: 0x%X, 0x%X\n", cipher1, cipher2);

  // Reverse the final permutation.
  uint8_t cipher1_no_perm, cipher2_no_perm;
  init_perm_.Permute(&cipher1, &cipher1_no_perm);
  init_perm_.Permute(&cipher2, &cipher2_no_perm);

  // Undo the feistel stuff.
  uint8_t right_state_c1 = cipher1_no_perm & 0x0F;
  uint8_t right_state_c2 = cipher2_no_perm & 0x0F;

  // Calculate the initial left state. We can just perform the inverse
  // final permutation, because that's the same as the initial permutation.
  uint8_t initial_left_state_c1, initial_left_state_c2;
  init_perm_.Permute(&plaintext, &initial_left_state_c1);
  init_perm_.Permute(&p_pair, &initial_left_state_c2);
  initial_left_state_c1 >>= 4;
  initial_left_state_c2 >>= 4;

  // Undo the XOR.
  uint8_t round_out_c1 = right_state_c1 ^ initial_left_state_c1;
  uint8_t round_out_c2 = right_state_c2 ^ initial_left_state_c2;

  // Undo the round function permutation.
  uint8_t sbox_out_c1, sbox_out_c2;
  inv_round_perm_.Permute(&round_out_c1, &sbox_out_c1);
  inv_round_perm_.Permute(&round_out_c2, &sbox_out_c2);

  printf("De-feisteled cipher texts: 0x%X, 0x%X\n", sbox_out_c1, sbox_out_c2);

  // Only look at the output from the left S-box.
  sbox_out_c1 >>= 2;
  sbox_out_c2 >>= 2;
  printf("S0 outputs: 0x%X, 0x%X\n", sbox_out_c1, sbox_out_c2);

  const uint8_t cipher_xor = sbox_out_c1 ^ sbox_out_c2;
  printf("Ciphertexts XOR: 0x%X\n", cipher_xor);

  // Check if we have a good pair.
  if (cipher_xor == output_xor_) {
    printf("Pair is GOOD!\n");
  } else {
    printf("Pair is BAD.\n");
  }
}

}  // namespace analysis
}  // namespace hw1
