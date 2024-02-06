// Copyright Espera Activa

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cinttypes>
#include <vector>
#include <iostream>

#include "FactWebApp.hpp"
#include "Factorizer.hpp"

#define NA_NUMBER -1
#define NEGATIVE_NUMBER -2

/// Constructor by default.
Factorizer::Factorizer() {
}

/// Destructor.
Factorizer::~Factorizer() {
}

/// Method to start consuming from the queue until a stop signal appears.
int Factorizer::run() {
  this->consumeForever();
  return 1;
}

/// Method who gets the next data to consume on
/// the numbers queue and send it to consume.
void Factorizer::consumeForever() {
  assert(this->consumingQueue);
  while (true) {
    // Get the next data to consume, or block while queue is empty
    const Numbers& currentNumber = this->consumingQueue->pop();
    // If data is the stop condition, stop the loop
    if ( currentNumber.numberIndex == -1 ) {
      break;
    }
    // Process this data
    this->consume(currentNumber);
  }
}

/// Calculate the prime factors of the current number.
void Factorizer::consume(Numbers currentNumber) {
  this->calculate_prime_factors(currentNumber.number,
    currentNumber.factorization);
    /// Produce in the factorization queue.
  this->produce(currentNumber);
}

void Factorizer::calculate_prime_factors(int64_t value,
  std::vector<int64_t>& factors) {
  // if value is_different from 1 or 0:
  if (value > 1) {
    // add_factors(value, 2, matrix, index)
    if (value%2 == 0) {
      add_factors(&value, 2, factors);
    }

    // for odd_numbers = 3 stop_when odd_numbers <= sqrt(value)
    for (int odd_numbers = 3; odd_numbers <= sqrt(value);
      odd_numbers = odd_numbers+2) {
      // add_factors(value, odd_numbers, matrix, index)
      if (value%odd_numbers == 0) {
        add_factors(&value, odd_numbers, factors);
      }
    }  // end for

    // if value > 2:
    if (value > 2) {
      // add value to matrix in index
      factors.push_back(value);
      factors.push_back(1);
    }  // end if
  } else if (value == 1 || value == 0) {
    factors.push_back(NA_NUMBER);
  } else if (value < 0) {
    //  add -1 to matrix in index
    factors.push_back(NEGATIVE_NUMBER);
  }  // end else
}  // end procedure

// procedure add_factors(current_value, current_factor, matrix, index) do
void Factorizer::add_factors(int64_t* current_value, int64_t current_factor,
  std::vector<int64_t>& factors) {
  // declare exponent := 0
  int exponent = 0;
  // while current_value divisible by current_factor:
  while (*current_value % current_factor == 0) {
    // if exponent equals 0:
    if (!exponent) {
      // add current_factor to matrix in index
      factors.push_back(current_factor);
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
  factors.push_back(exponent);
}  // end procedure
