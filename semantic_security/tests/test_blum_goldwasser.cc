#include <stdint.h>
#include <stdio.h>

#include "gtest/gtest.h"

#include "semantic_security/blum_goldwasser.h"

namespace hw1 {
namespace semantic_security {
namespace testing {
namespace {

// Parameters to use for Blum-Goldwasser algorithm.
const uint64_t kP = 499;
const uint64_t kQ = 547;
const int64_t kA = -57;
const int64_t kB = 52;
const uint64_t kX0 = 159201;

}  // namespace

// Tests for the Blum-Goldwasser algorithm.
class BgTest : public ::testing::Test {
 protected:
  BgTest() : bg_(kP, kQ, kA, kB, kX0) {}

  // Blum-Goldwasser instance to use for testing.
  BlumGoldwasser bg_;
};

// Tests that we can encrypt and decrypt a message successfully.
TEST_F(BgTest, EncryptionDecryptionTest) {
  // Create a test message to encrypt.
  const uint8_t test_message[] = {0x9C, 0x10, 0xC0};

  // Encrypt the message.
  uint8_t cipher[3];
  const uint64_t x_t = bg_.Encrypt(test_message, 3, cipher);
  printf("Encrypted: 0x%X 0x%X 0x%X\n", cipher[0], cipher[1], cipher[2]);
  printf("Last X: %lu\n", x_t);

  // Decrypt the message.
  uint8_t decrypted[3];
  bg_.Decrypt(cipher, 3, x_t, decrypted);

  // Check that the decryption is the same.
  for (uint8_t i = 0; i < 3; ++i) {
    EXPECT_EQ(test_message[i], decrypted[i]);
  }
}

}  // namespace testing
}  // namespace semantic_security
}  // namespace hw1
