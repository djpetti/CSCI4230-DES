#ifndef HW1_S_BOX_H_
#define HW1_S_BOX_H_

#include <stdint.h>

namespace hw1 {

// A class that implements substitution.
class SBox {
 public:
  // Args:
  //  box: Array representing the box contents in row-major format.
  //  row_size: Number of rows in the box.
  //  col_size: Number of columns in the box.
  SBox(const uint8_t *box, uint8_t row_size, uint8_t col_size);
  ~SBox();

  // Looks up a particular value in the box.
  // Args:
  //  row_addr: The row address of the value.
  //  col_addr: The column address of the value.
  // Returns:
  //  The value from the S box.
  uint8_t LookUp(uint16_t row_addr, uint16_t col_addr);

 private:
  // Stores the actual box information.
  uint8_t *box_;
  // Number of rows.
  uint8_t row_size_;
  // Number of columns.
  uint8_t col_size_;
};

}  // namespace hw1

#endif  // HW1_S_BOX_H_
