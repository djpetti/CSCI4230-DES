#include "blum_goldwasser_public.h"

#include <math.h>
#include <string.h>

#include "numerical/mod.h"

namespace hw1 {
namespace semantic_security {

BlumGoldwasserPublic::BlumGoldwasserPublic(uint64_t n, uint64_t x_0)
    : n_(n), x_t_(x_0) {}

uint64_t BlumGoldwasserPublic::Encrypt(const uint8_t *plain, uint32_t length,
                                       uint8_t *cipher) {
  XorKeyStream(plain, length, cipher);

  // Return the next generated number.
  return x_t_;
}

uint64_t BlumGoldwasserPublic::BlockSize() const {
  const uint64_t k = log2(n_);
  return log2(k);
}

uint64_t BlumGoldwasserPublic::GetRandom() {
  // Update the state.
  x_t_ = numerical::Power(x_t_, 2, n_);
  return x_t_;
}

uint8_t *BlumGoldwasserPublic::GenKeyStream(uint64_t block_size,
                                            uint32_t length) {
  // Make sure our buffer can hold enough data.
  uint32_t stream_size = length / sizeof(uint64_t);
  if (length % sizeof(uint64_t)) {
    ++stream_size;
  }
  key_stream_.reserve(stream_size);
  uint64_t *stream = key_stream_.data();

  // Generate each block individually.
  uint8_t offset = 0;
  uint32_t i = 0;
  uint32_t processed_length = 0;
  uint64_t key = GetRandom();
  uint64_t key_offset = key;
  while (processed_length < length * 8) {
    // Make sure the bits are in a defined state before we try to set them.
    const uint64_t clear = ~(-1 << offset);
    stream[i] &= clear;
    // Set the key bits.
    stream[i] |= key_offset;

    // Next time, we want to write the next block.
    offset += block_size;
    processed_length += block_size;
    if (offset > sizeof(uint64_t) * 8) {
      // We have to transition to the next integer in the array.
      ++i;
      offset %= sizeof(uint64_t) * 8;

      // We have to continue adding the last part of our key.
      key_offset = key >> (block_size - offset);
    } else {
      // Generate a new key.
      key = GetRandom();
      key_offset = key << offset;
    }
  }

  return reinterpret_cast<uint8_t *>(stream);
}

void BlumGoldwasserPublic::XorKeyStream(const uint8_t *message, uint32_t length,
                                        uint8_t *output) {
  // Compute block size.
  const uint64_t h = BlockSize();
  // Generate the key stream.
  const uint8_t *key_stream = GenKeyStream(h, length);

  // XOR plaintext with the key stream.
  for (uint32_t i = 0; i < length; ++i) {
    output[i] = key_stream[i] ^ message[i];
  }
}

}  // namespace semantic_security
}  // namespace hw1
