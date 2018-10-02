#ifndef HW1_DES_H_
#define HW1_DES_H_

#include <stdint.h>

#include <vector>

#include "feistel_cipher.h"
#include "key_gen.h"

namespace hw1 {

// Implements toy DES.
class Des {
 public:
  // Args:
  //  key: The 10-bit key to use for encryption and decryption, as a 2-length
  //       byte array.
  //  rounds: Optionally, allows us to manually specify the number of rounds.
  //          The default is 2.
  Des(const uint8_t *key, uint8_t rounds = 2);

  // Changes the key to use for encryption and decryption.
  // Args:
  //  key: The key to set, as a 2-length byte array.
  void SetKey(const uint8_t *key);

  // Encrypts a new message.
  // Args:
  //  plain: The message to encrypt.
  //  length: The length in bytes of the message.
  //  cipher: Where to write the encrypted message.
  void Encrypt(const char *plain, uint32_t length, char *cipher);
  // Decrypts a message.
  // Args:
  //  cipher: The message to decrypt.
  //  length: The length of the message.
  //  plain: Where to write the decrypted message.
  void Decrypt(const char *cipher, uint32_t length, char *plain);

 private:
  // Generates the list of subkeys, if we haven't done so already.
  void GenSubKeys();
  // Encrypts a single block.
  // Args:
  //  plain: The plaintext block.
  // Returns:
  //  The ciphertext block.
  uint8_t EncryptBlock(uint8_t plain);
  // Decrypts a single block.
  // Args:
  //  cipher: The ciphertext block.
  // Returns:
  //  The plaintext block.
  uint8_t DecryptBlock(uint8_t cipher);

  // Number of rounds to perform.
  uint8_t num_rounds_;

  // The feistel cipher to use internally for encryption and decyption.
  FeistelCipher cipher_;
  // The key generator to use for producing subkeys.
  KeyGen key_gen_;

  // List of subkeys to use.
  ::std::vector<uint8_t> subkeys_;
};

}  // namespace hw1

#endif  // HW1_DES_H_
