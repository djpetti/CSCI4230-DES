#ifndef HW1_NUMERICAL_POLY_H_
#define HW1_NUMERICAL_POLY_H_

#include <stdint.h>

#include <vector>

namespace hw1 {
namespace numerical {

// Represents a modular polynomial.
class Poly {
 public:
  // Args:
  //  coefficients: The coefficients, in order from highest to lowest power.
  //  modulus: The modulus to use.
  Poly(const ::std::vector<uint64_t> &coefficients, uint64_t modulus);

  // Evaluates the polynomial at a given value.
  // Args:
  //  value: The value to evaluate at.
  // Returns:
  //  The result of the evaluation.
  uint64_t Evaluate(uint64_t value);
  // Sets a new modulus.
  // Args:
  //  modulus: New modulus to set.
  void SetModulus(uint64_t modulus);

 private:
  // Internal storage for the coefficients.
  ::std::vector<uint64_t> coefficients_;
  // The modulus to use.
  uint64_t modulus_;
};

}  // namespace numerical
}  // namespace hw1

#endif  // HW1_NUMERICAL_POLY_H_
