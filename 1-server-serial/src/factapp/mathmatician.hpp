// Copyright [2021] Ariel Solis, ariel.solismonge@ucr.ac.cr
#ifndef MATHMATICIAN_H
#define MATHMATICIAN_H

#include <cstdint>
#include <vector>

class Mathmatician {
 public:
/**
 * @brief Constructor for the class Mathmatician.
 */ 
  Mathmatician();

/**
 * @brief Calculates all the prime factors of a number
 * 
 * @details The number has to be more than 1 to have prime factors.
 *  first we see if the number is divisible by 2, if it is we save
 *  2 in the array and set the exponent to 1, then divide the number by 2.
 *  If the number is divisible by 2 again we add 1 to the exponent.
 *  When the number is no longer divisible by 2 we add the exponent next
 *  to the first 2.
 *  After that we know for a fact, the number is odd, so we 
 *  divide it by only odd numbers and sabe de divisible ones in the array
 *  After that process we have sucessfully calculated all factors.
 * 
 * @param value 
 * @param matrix 
 * @param matrix_index 
 */
  void calculate_prime_factors(int64_t value,
  std::vector<std::vector<int64_t>>& matrix, size_t matrix_index);

/**
 * @brief Method to calculate the square root of a number with integer
 * precision
 * 
 * @param value Número a procesar
 * @return r La raiz cuadrada entera de valor
 */
int64_t isqrt(int64_t value);

 private:
/**
 * @brief Method to add the factors of the factorization at the vector, including the exponent.
 * 
 * @param int64_t* current_value
 * @param int64_t current_factor
 * @param std::vector<std::vector<int64_t>>& matrix
 * @param size_t matrix_index
 */ 
  void add_factors(int64_t* current_value, int64_t current_factor
  , std::vector<std::vector<int64_t>>& matrix, size_t matrix_index);
};

#endif  // MATHMATICIAN_H
