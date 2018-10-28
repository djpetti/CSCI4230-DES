#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "miller_rabin.h"

// Threshold to use for primality checks.
const float kPrimeThreshold = 0.99999;

// Prints usage information and exits the program.
// Args:
//  name: The name of the executable.
void PrintUsageAndExit(const char *name) {
  printf("Usage: %s number\n", name);
  printf("Number must be odd and larger than 3.\n");
  exit(1);
}

int main(int argc, const char **argv) {
  if (argc != 2) {
    // Wrong number of arguments.
    PrintUsageAndExit(argv[0]);
  }

  const uint32_t number = atoi(argv[1]);
  if (number <= 3 || number % 2 == 0) {
    PrintUsageAndExit(argv[0]);
  }

  // Run the Miller-Rabin check.
  ::hw1::makeup_exam::MillerRabin checker(kPrimeThreshold);
  if (checker.IsPrime(number)) {
    printf("%u is prime.\n", number);
  } else {
    printf("%u is composite.\n", number);
  }

  return 0;
}
