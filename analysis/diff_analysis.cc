#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "des/des.h"

// Simple tool for performing differential crytanalysis.

// Prints a help message and exits the program.
// Args:
//  program: The name of the program.
void PrintHelpAndExit(const char *program) {
  printf("Usage: %s key input_xor output_xor\n", program);
  printf("Key must be two characters, and xors must be one byte.\n");
  exit(1);
}

// Waits for a user input, and checks if it matches the input and output xors.
// Args:
//  des: The DES instance to use.
//  input_xor: The target input xor.
//  output_xor: The target output xor.
void TryInput(::hw1::Des *des, uint8_t input_xor, uint8_t output_xor) {
  printf("Enter 1-byte plaintext:\n");
  uint8_t plaintext;
  scanf("%hhu", &plaintext);

  // Compute the pair.
  uint8_t p_pair = plaintext ^ input_xor;
  printf("Plaintext pair: 0x%X\n", p_pair);

  // Compute the ciphertexts.
  uint8_t cipher1, cipher2;
  des->Encrypt((const char *)&plaintext, 1, (char *)&cipher1);
  des->Encrypt((const char *)&p_pair, 1, (char *)&cipher2);
  printf("Ciphertexts: 0x%X, 0x%X\n", cipher1, cipher2);

  // Check if we have a good pair.
  if ((cipher1 ^ cipher2) == output_xor) {
    printf("Pair is GOOD!\n");
  } else {
    printf("Pair is BAD.\n");
  }
}

int main(int argc, const char **argv) {
  // Get CLI arguments.
  if (argc != 4) {
    PrintHelpAndExit(argv[0]);
  }

  const char *key = argv[1];
  if (strlen(key) != 2) {
    // Key is not the right length.
    PrintHelpAndExit(argv[0]);
  }

  const uint8_t input_xor = atoi(argv[2]);
  const uint8_t output_xor = atoi(argv[3]);

  ::hw1::Des des((const uint8_t *)key);

  while (true) {
    TryInput(&des, input_xor, output_xor);
  }

  return 0;
}
