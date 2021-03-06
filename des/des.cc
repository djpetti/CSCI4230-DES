#include "des.h"

namespace hw1 {

Des::Des(const uint8_t *key, uint8_t rounds /*=2*/)
    : num_rounds_(rounds), key_gen_(key) {}

void Des::SetKey(const uint8_t *key) {
  // Reset the key generator.
  key_gen_.Reset(key);
  // Clear the generated subkeys.
  subkeys_.clear();
}

void Des::Encrypt(const char *plain, uint32_t length, char *cipher) {
  GenSubKeys();

  // Encrypt block-by-block. Lucky for us, each block is one byte.
  for (uint32_t i = 0; i < length; ++i) {
    cipher[i] = EncryptBlock((uint8_t)plain[i]);
  }
}

void Des::Decrypt(const char *cipher, uint32_t length, char *plain) {
  GenSubKeys();

  // Decrypt block-by-block.
  for (uint32_t i = 0; i < length; ++i) {
    plain[i] = DecryptBlock((uint8_t)cipher[i]);
  }
}

void Des::GenSubKeys() {
  while (subkeys_.size() < num_rounds_) {
    // Generate subkeys.
    subkeys_.push_back(key_gen_.GenNext());
  }
}

uint8_t Des::EncryptBlock(uint8_t plain) {
  // Use the cipher to encrypt it.
  cipher_.SetInput(plain);

  for (const auto key : subkeys_) {
    cipher_.DoEncRound(key);
  }

  return cipher_.GetOutput();
}

uint8_t Des::DecryptBlock(uint8_t cipher) {
  // Use the cipher to decrypt it.
  cipher_.SetInput(cipher);

  // For decryption, we need to use the subkeys in reverse.
  for (int32_t i = subkeys_.size() - 1; i >= 0; --i) {
    cipher_.DoDecRound(subkeys_[i]);
  }

  return cipher_.GetOutput();
}

}  // namespace hw1
