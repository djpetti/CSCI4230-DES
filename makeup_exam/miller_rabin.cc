#include "miller_rabin.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "numerical/mod.h"

namespace hw1 {
namespace makeup_exam {
namespace {

// Runs a single test iteration.
// Args:
//  number: The number to test.
//  k: The k value to use.
//  q: The q value to use.
// Returns:
//  True if number might be prime, false otherwise.
bool CheckOnce(uint64_t number, uint64_t k, uint64_t q) {
  // Select a random integer.
  uint64_t a = ((uint64_t)rand() << 32) | rand();
  a %= number - 3;
  a += 2;

  if (numerical::Power(a, q, number) == 1) {
    return true;
  }

  for (uint64_t i = 0; i < k; ++i) {
    uint64_t exp = (uint64_t)1 << i;
    exp *= q;

    if (numerical::Power(a, exp, number) == number - 1) {
      return true;
    }
  }

  return false;
}

}  // namepspace

MillerRabin::MillerRabin(float threshold) : threshold_(threshold) {
  // Seed the random number generator.
  srand(time(NULL));
}

bool MillerRabin::IsPrime(uint64_t number) {
  // Compute k and q values for the number.
  const uint64_t k = ComputeK(number);
  const uint64_t q = (number - 1) / ((uint64_t)1 << k);

  // Run iterations until we are confident enough.
  float confidence = 0;
  uint8_t iters = 0;
  while (confidence < threshold_) {
    if (!CheckOnce(number, k, q)) {
      // It's certainly not prime.
      return false;
    }

    confidence = 1.0 - 1.0 / (1 << (2 * ++iters));
  }

  return true;
}

uint64_t MillerRabin::ComputeK(uint64_t number) {
  const uint64_t goal = number - 1;

  uint64_t result = 2;
  uint64_t k = 1;
  while (result <= goal) {
    if (goal % result == 0 && (goal / result) % 2 == 1) {
      return k;
    }

    result = (uint64_t)1 << ++k;
  }

  // This shouldn't happen.
  assert(false && "Could not find k value.");
}

}  // namespace makeup_exam
}  // namespace hw1
