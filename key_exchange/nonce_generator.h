#ifndef HW1_KEY_EXCHANGE_NONCE_GENERATOR_H_
#define HW1_KEY_EXCHANGE_NONCE_GENERATOR_H_

#include <stdint.h>

namespace hw1 {
namespace key_exchange {

// A simple class to help with nonce generation.
class NonceGenerator {
 public:
  NonceGenerator();

  // Generate a new nonce.
  // Returns:
  //  The generated nonce.
  uint32_t Generate();

 private:
  // The state of the generator.
  uint32_t state_;
};

}  // namespace key_exchange
}  // namespace hw1

#endif  // HW1_KEY_EXCHANGE_NONCE_GENERATOR_H_
