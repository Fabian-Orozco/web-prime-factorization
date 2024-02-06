// Copyright Espera Activa

/**
 * @file NumbersStructure.hpp
 * @brief Struct containing data identifying its value, its httpRequest and index as well as a method to compare instances
 * @author Team Espera Activa
 * @date 12/2021
 */

#ifndef NUMBERS_STRUCTURE_
#define NUMBERS_STRUCTURE_

#include <cstdlib>
#include <vector>

#include "HttpReqRes.hpp"
#include "HttpRequest.hpp"

struct Numbers {
  size_t numbersCount = 0;  ///< number of values requested in the HttpRequest
  HttpReqRes myRequest;  ///< the current request
  int requestNumber = 0;  ///< the number assigned to this request
  int numberIndex = 0;  ///< stores the position of this number HttpRequest
  int64_t status = 0;  ///< if the requested value is valid or invalid
  int64_t number = 0;  ///< the value
  std::vector<int64_t> factorization;  ///< containing the factorization of
  ///< the number

  /**
   * @brief A method that overloads a comparison operator, 
   * so that is possible to compare the data contained inside two numbers
   * 
   * @param other the second object to compare
   * @return true if the two object numbers are the same
   * @return false if not the same object numbers
   */
  bool operator==(const Numbers other) const {
    return number == other.number;
  }
};

#endif  // NUMBERS_STRUCTURE_
