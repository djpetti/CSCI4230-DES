#ifndef HW1_NUMERICAL_EUCLID_H_
#define HW1_NUMERICAL_EUCLID_H_

#include <stdint.h>

namespace hw1 {
namespace numerical {

// Uses Euclid's algorithm to determine the GCD of two numbers.
// Args:
//  dividend: The first number.
//  divisor: The second number.
// Returns: The GCD of the two numbers.
uint64_t Gcd(uint64_t dividend, uint64_t divisor);

}  // namespace numerical
}  // namespace hw1

#endif  // HW1_NUMERICAL_EUCLID_H_
