#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "diff_analysis.h"

// Simple tool for performing differential crytanalysis.

// Prints a help message and exits the program.
// Args:
//  program: The name of the program.
void PrintHelpAndExit(const char *program) {
  printf("Usage: %s key input_xor output_xor\n", program);
  printf("Key must be two characters, and xors must be one byte.\n");
  exit(1);
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

  ::hw1::analysis::Analyzer analyzer(input_xor, output_xor,
                                     (const uint8_t *)key);

  while (true) {
    printf("Enter 1-byte plaintext:\n");
    uint8_t plaintext;
    scanf("%hhu", &plaintext);

    analyzer.TryInput(plaintext);
  }

  return 0;
}
