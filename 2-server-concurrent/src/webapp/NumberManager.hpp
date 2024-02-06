// Copyright Espera Activa

 /**
 * @file NumberManager.hpp
 * @brief 
 * @author Team Espera Activa
 * @date 12/2021
 */

#ifndef NUMBER_MANAGER_
#define NUMBER_MANAGER_

#include <string>
#include <vector>
#include <cinttypes>
#include "HttpRequest.hpp"

#define VALID_NUMBER 1
#define INVALID_NUMBER 0

class NumberManager {
 public:
  std::vector<int64_t> numbers;  ///< numbers for factorization.
  std::vector<int64_t> status;  ///< valid or not valid of those numbers.

  /**
  * @brief Takes the vector of strings created previously to convert all
  * the numbers to int64 to be factorized.
  * 
  * @param counter 
  * @param words 
  * @param sem 
  * @param aux 
  * @param limite 
  */
  void setNumbers(std::size_t& counter, std::vector<std::string>& words,
    bool& sem, int64_t aux, int64_t limite);

  /**
  * @brief This method takes the input and separates the numbers from the comas
  * ans adds them to a vector of strings
  * 
  * @param line 
  * @param counter 
  * @param words 
  * @param sem 
  */
  void separateNumbers(std::string& line, std::size_t& counter,
    std::vector<std::string>& words, bool& sem);

  /**
   * @brief This method takes a string in the form 1,32,4,6,5,41 and separates
   * each number from the strng and adds it to the status vector and numbers
   * vector respectively.
   * 
   * @param numbersString
   */
  void extractNumbers(std::string numbersString);

  /**
   * @brief Get access to the numbers vector.
   */
  inline const std::vector<int64_t>& getHttpNumbers()  { return this->numbers; }

  /**
   * @brief Get access to the status vector.
   */
    inline const std::vector<int64_t>& getHttpStatus()  { return this->status; }
};
#endif  // NUMBER_MANAGER_
