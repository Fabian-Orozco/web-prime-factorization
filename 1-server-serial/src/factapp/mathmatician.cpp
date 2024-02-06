// Copyright 2021 Ariel Solis, ariel.solismonge@ucr.ac.cr

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cinttypes>
#include <vector>
#include "mathmatician.hpp"

#define NA_NUMBER -1
#define NEGATIVE_NUMBER -2

Mathmatician::Mathmatician() {
}

// Adapted from
// <https://www.geeksforgeeks.org/print-all-prime-factors-of-a-given-number/>
// procedure calculate_prime_factors(value, matrix, index):
void Mathmatician::calculate_prime_factors(int64_t value,
  std::vector<std::vector<int64_t>>& matrix, size_t matrix_index) {
  // if value is_different from 1 or 0:
  if (value > 1) {
    // add_factors(value, 2, matrix, index)
    if (value%2 == 0) {
      add_factors(&value, 2, matrix, matrix_index);
    }

    // for odd_numbers = 3 stop_when odd_numbers <= sqrt(value)
    for (int odd_numbers = 3; odd_numbers <= isqrt(value);
      odd_numbers = odd_numbers+2) {
      // add_factors(value, odd_numbers, matrix, index)
      if (value%odd_numbers == 0) {
        add_factors(&value, odd_numbers, matrix, matrix_index);
      }
    }  // end for

    // if value > 2:
    if (value > 2) {
      // add value to matrix in index
      matrix[matrix_index].push_back(value);
      matrix[matrix_index].push_back(1);
    }  // end if
  } else if (value == 1 || value == 0) {
    matrix[matrix_index].push_back(NA_NUMBER);
  } else if (value < 0) {
  // add -1 to matrix in index
    matrix[matrix_index].push_back(NEGATIVE_NUMBER);
  }  // end else
}  // end procedure

// procedure add_factors(current_value, current_factor, matrix, index) do
void Mathmatician::add_factors(int64_t* current_value, int64_t current_factor,
  std::vector<std::vector<int64_t>>& matrix, size_t matrix_index) {
  // declare exponent := 0
  int exponent = 0;
  // while current_value divisible by current_factor:
  while (*current_value % current_factor == 0) {
    // if exponent equals 0:
    if (!exponent) {
      // add current_factor to matrix in index
      matrix[matrix_index].push_back(current_factor);
      // set exponent to 1
      exponent = 1;
    // else
    } else {
      // increment exponent by 1
      ++exponent;
    }  // end if
    // current_value = current_value/current_factor
    *current_value = *current_value/current_factor;
  }  // end while
  // add exponent to matrix in index
  matrix[matrix_index].push_back(exponent);
}  // end procedure

// Adapted from:
// http://rosettacode.org/wiki/Isqrt_(integer_square_root)_of_X#C.2B.2B
int64_t Mathmatician::isqrt(int64_t value) {
  if (value < 0 || value >= 9223372036854775807) {
    return value;
  }
  int64_t q = 1;
  int64_t r = 0;
  while (q <= value) {
    q <<= 2;
  }

  while (q > 1) {
    int64_t t = 0;
    q >>= 2;
    t = value - r - q;
    r >>= 1;
    if (t >= 0) {
      value = t;
      r += q;
    }
  }
  return r;
}
