// Copyright Espera Activa

/**
 * @file Factorizer.hpp
 * @brief subclass of threads, will consume numbers and produce factorizations
 * @author Team Espera Activa
 * @date 12/2021
 */

#ifndef FACTORIZER_
#define FACTORIZER_

#include <vector>

#include "Assembler.hpp"
#include "FactWebApp.hpp"
#include "NumbersStructure.hpp"
#include "Thread.hpp"

class Factorizer : public Assembler<Numbers, Numbers> {
 private:
  // Adapted from
  // <https://www.geeksforgeeks.org/print-all-prime-factors-of-a-given-number/>

  // procedure calculate_prime_factors(value, matrix, index):
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
   * @param value It's the number to process
   * @param factors vector in which the results are written
   */
  void calculate_prime_factors(int64_t value
    , std::vector<int64_t>& factors);

  /**
   * @brief Adds factors inside the vector 'factors', taking into account exponents.
   * 
   * @param current_value 
   * @param current_factor 
   * @param factors 
   */
  void add_factors(int64_t* current_value, int64_t current_factor
    , std::vector<int64_t>& factors);

 protected:
  /**
   * @brief starts this thread
   * 
   * @return int the exit status
   */
  int run();

  /**
   * 
   * @brief Consumes an object of type numbers from the Queue (numbersQueue)
   * 
   * @param number the data to be consumed
   */
  void consume(Numbers number);

  /**
   * @brief Keeps this threads active until the server is closed,
   * so that any new number/request can be consumed
   * 
   */
  void consumeForever();

 public:
   /**
   * @brief Constructor by default.
   * 
   */
  Factorizer();
    /**
   * @brief Destructor.
   */
  ~Factorizer();
};
#endif  // FACTORIZER_
