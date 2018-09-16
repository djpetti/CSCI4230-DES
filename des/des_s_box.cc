#include "des_s_box.h"

namespace hw1 {

DesSBox::DesSBox(const uint8_t *box) : SBox(box, 4, 4) {}

uint8_t DesSBox::Substitute(uint8_t input) {
  // Bits one and four are the row address.
  const uint8_t row_addr = ((input & 0x8) >> 2) | (input & 0x1);
  // Bits two and three are the column address.
  const uint8_t col_addr = (input & 0x6) >> 1;

  return LookUp(row_addr, col_addr);
}

}  // namespace hw1
