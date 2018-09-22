#ifndef HW1_ANALYSIS_DIFF_ANALYSIS_H_
#define HW1_ANALYSIS_DIFF_ANALYSIS_H_

#include <stdint.h>

#include "des/common/p_box.h"
#include "des/des.h"
#include "des/expansion.h"

namespace hw1 {
namespace analysis {

// Class that helps with differential analysis.
class Analyzer {
 public:
  // Args:
  //  input_xor: The input XOR we are trying to match.
  //  output_xor: The output XOR we are trying to match.
  //  key: The 10-bit key to use for encryption, as a 2-length byte array.
  Analyzer(uint8_t input_xor, uint8_t output_xor, const uint8_t *key);

  // Checks if an input matches the input and output XORs.
  // Args:
  //  plaintext: The plaintext input that we want to try to match.
  void TryInput(uint8_t plaintext);

 private:
  // The target input XOR.
  uint8_t input_xor_;
  // The target output XOR.
  uint8_t output_xor_;

  // The DES instance to use for encryption.
  Des des_;

  // The initial permutation.
  PBox init_perm_;
  // The final permutation.
  PBox final_perm_;
  // The round function left expansion.
  PBox round_left_exp_;
  // The inverse of the round function left expansion.
  PBox inv_round_left_exp_;
  // The inverse round function permutation.
  PBox inv_round_perm_;
};

}  // namespace analysis
}  // namespace hw1

#endif  // HW1_ANALYSIS_DIFF_ANALYSIS_H_
