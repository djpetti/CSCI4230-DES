#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "des.h"

namespace {

// Key to use for encryption.
const uint8_t kKey[] = {0xFA, 0x03};
// Message to encrypt.
const char *kMessage = "Secret Message";

// Prints a char array in hex representation.
// Args:
//  array: The array to print.
//  length: The length of the array.
void PrintHex(const char *array, uint32_t length) {
  for (uint32_t i = 0; i < length; ++i) {
    printf("%x", (uint8_t)array[i]);
  }
  printf("\n");
}

}  // namespace

int main() {
  ::hw1::Des des(kKey);

  const uint32_t length = strlen(kMessage) + 1;
  printf("Original message: %s\n", kMessage);

  // Encrypt the message.
  char encrypted[length];
  des.Encrypt(kMessage, length, encrypted);

  // Print the encrypted version.
  printf("Cyphertext: ");
  PrintHex(encrypted, length);

  // Decrypt the message.
  char decrypted[length];
  des.Decrypt(encrypted, length, decrypted);

  printf("Plaintext: %s\n", decrypted);
}
