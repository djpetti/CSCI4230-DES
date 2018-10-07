#include "nonce_generator.h"

#include <time.h>

namespace hw1 {
namespace key_exchange {

// Initialize state with the time.
NonceGenerator::NonceGenerator() : state_(time(NULL)) {}

uint32_t NonceGenerator::NonceGenerator::Generate() {
  // Return a monotonically increasing count.
  return state_++;
}

}  // namespace key_exchange
}  // namespace hw1
