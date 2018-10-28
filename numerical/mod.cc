#include "mod.h"

namespace hw1 {
namespace numerical {

uint64_t Power(uint64_t base, uint64_t exp, uint64_t mod) {
  uint64_t pow = 1;
  for (uint8_t i = 0; i < 64; ++i) {
    const uint8_t bit = 63 - i;

    pow *= pow;
    pow %= mod;
    if (exp & ((uint64_t)1 << bit)) {
      pow *= base;
      pow %= mod;
    }
  }

  return pow;
}

}  // namespace numerical
}  // namespace hw1
