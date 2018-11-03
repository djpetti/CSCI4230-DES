#include "blum_goldwasser.h"

#include "numerical/mod.h"

namespace hw1 {
namespace semantic_security {

BlumGoldwasser::BlumGoldwasser(uint64_t p, uint64_t q, int64_t a, int64_t b,
                               uint64_t x_0)
    : BlumGoldwasserPublic(p * q, x_0), p_(p), q_(q), a_(a), b_(b) {}

void BlumGoldwasser::Decrypt(const uint8_t *cipher, uint32_t length,
                             uint64_t last_x, uint8_t *plain) {
  // First, we need to correctly initialize the state of the RNG.
  ReconstructRngState(last_x, length);

  // Now, we can generate the same key stream we used when encrypting.
  XorKeyStream(cipher, length, plain);
}

void BlumGoldwasser::ReconstructRngState(uint64_t last_x, uint32_t length) {
  // First, determine the number of blocks in the message.
  const uint64_t block_size = BlockSize();
  const uint32_t bit_length = length * 8;
  uint32_t num_blocks = bit_length / block_size;
  if (bit_length % block_size != 0) {
    num_blocks += 1;
  }

  // Reconstruct x_0.
  const uint64_t d1 = numerical::Power((p_ + 1) / 4, num_blocks + 1, p_ - 1);
  const uint64_t d2 = numerical::Power((q_ + 1) / 4, num_blocks + 1, q_ - 1);
  const int64_t u = numerical::Power(last_x, d1, p_);
  const int64_t v = numerical::Power(last_x, d2, q_);

  const int64_t sp = static_cast<int64_t>(p_);
  const int64_t sq = static_cast<int64_t>(q_);
  x_t_ = numerical::Mod(v * a_ * sp + u * b_ * sq, n_);
}

}  // namespace semantic_security
}  // namespace hw1
