#ifndef HW1_SEMANTIC_SECURITY_BLUM_GOLDWASSER_PUBLIC_H_
#define HW1_SEMANTIC_SECURITY_BLUM_GOLDWASSER_PUBLIC_H_

#include <stdint.h>

#include <vector>

namespace hw1 {
namespace semantic_security {

// Implements the parts of Blum-Goldwasser that require only a public key.
class BlumGoldwasserPublic {
 public:
  // Args:
  //  n: The public key.
  //  x_0: The seed to use for message encryption.
  BlumGoldwasserPublic(uint64_t n, uint64_t x_0);
  virtual ~BlumGoldwasserPublic() = default;

  // Encrypts a message.
  // Args:
  //  plain: The message, as a uint8_t array.
  //  length: The length of the message, in bytes.
  //  cipher: Set to the encrypted ciphertext. Should have at least length bytes
  //          of space.
  // Returns:
  //  x_t+1 value that will be used to reconstruct the seed.
  virtual uint64_t Encrypt(const uint8_t *plain, uint32_t length,
                           uint8_t *cipher);

 protected:
  // Computes the size of a message block.
  // Returns:
  //  The size of a block.
  uint64_t BlockSize() const;
  // Generates the key stream.
  // Args:
  //  block_size: The size of the block that we are using for encryption.
  //  length: The length of the message, in bytes.
  // Returns:
  //  The key stream, which is at least length bytes long.
  uint8_t *GenKeyStream(uint64_t block_size, uint32_t length);
  // Generates a key stream and XORs it with a message.
  // Args:
  //  message: The message to XOR with.
  //  length: The length of the message in bytes.
  //  output: Where to write the output.
  void XorKeyStream(const uint8_t *message, uint32_t length, uint8_t *output);

  // The public key.
  uint64_t n_;
  // The random state.
  uint64_t x_t_;

 private:
  // Computes the next random number.
  // Returns:
  //  The next random number.
  uint64_t GetRandom();

  // Internal buffer for storing the key stream.
  ::std::vector<uint64_t> key_stream_;
};

}  // namespace semantic_security
}  // namespace hw1

#endif  // HW1_SEMANTIC_SECURITY_BLUM_GOLDWASSER_PUBLIC_H_
