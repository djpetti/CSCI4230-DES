#ifndef HW1_NUMERICAL_MOD_H_
#define HW1_NUMERICAL_MOD_H_

#include <stdint.h>

namespace hw1 {
namespace numerical {

// Performs modular exponentiation.
// Args:
//  base: The base.
//  exp: The exponent.
//  mod: The modulus.
// Returns:
//  base ^ exp % mod
uint64_t Power(uint64_t base, uint64_t exp, uint64_t mod);

}  // namespace numerical
}  // namespace hw1

#endif  // HW1_NUMERICAL_MOD_H_
