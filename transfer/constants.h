#ifndef HW1_TRANSFER_CONSTANTS_H_
#define HW1_TRANSFER_CONSTANTS_H_

namespace hw1 {
namespace transfer {

// Size of message header, in bytes.
const uint8_t kMessageHeaderSize = 36;
// Size of a file chunk, in bytes.
const uint32_t kChunkSize = 1024;

}  // namespace transfer
}  // namespace hw1

#endif  // HW1_TRANSFER_CONSTANTS_H_
