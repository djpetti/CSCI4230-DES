#ifndef HW1_MAKEUP_EXAM_MILLER_RABIN_H_
#define HW1_MAKEUP_EXAM_MILLER_RABIN_H_

#include <stdint.h>

namespace hw1 {
namespace makeup_exam {

// Implements the Miller-Rabin primality testing algorithm.
class MillerRabin {
 public:
  // Args:
  //  threshold: How sure we have to be of our predictions before making a
  //  determination.
  MillerRabin(float threshold);

  // Determines whether a number is prime.
  // Args:
  //  number: The number to check.
  // Returns:
  //  True if the number is prime, false otherwise.
  bool IsPrime(uint64_t number);

 private:
  // Computes the value of k.
  // Args:
  //  number: The number to compute it for.
  // Returns:
  //  The value of k.
  uint64_t ComputeK(uint64_t number);

  // The threshold value.
  float threshold_;
};

}  // namespace makeup_exam
}  // namespace hw1

#endif  // HW1_MAKEUP_EXAM_MILLER_RABIN_H_
