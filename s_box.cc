#include "s_box.h"

#include <string.h>

namespace hw1 {

SBox::SBox(const uint8_t *box, uint8_t row_size, uint8_t col_size)
    : row_size_(row_size), col_size_(col_size) {
  // Copy the box information.
  box_ = new uint8_t[row_size * col_size];
  memcpy(box_, box, row_size * col_size);
}

SBox::~SBox() {
  // Free the box information.
  delete[] box_;
}

uint8_t SBox::LookUp(uint16_t row_addr, uint16_t col_addr) {
  // Find the value.
  const uint16_t index = row_addr * row_size_ + col_addr;
  return box_[index];
}

}  // namespace hw1
