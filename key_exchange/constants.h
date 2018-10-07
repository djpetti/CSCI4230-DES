#ifndef HW1_TRANSFER_CONSTANTS_H_
#define HW1_TRANSFER_CONSTANTS_H_

namespace hw1 {
namespace key_exchange {

// Maximum chunk size, in bytes.
const uint32_t kChunkSize = 1024;

// Size of the session key message, in bytes.
const uint8_t kKeyMessageSize = 2 * 2 /* Key size. */ + 2 * 1 /* Client ID size.
                                                               */
                                + 4 * 2;                      /* Nonce size. */

}  // namespace key_exchange
}  // namespace hw1

#endif  // HW1_TRANSFER_CONSTANTS_H_
