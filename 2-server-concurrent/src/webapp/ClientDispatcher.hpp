// Copyright Espera Activa

/**
 * @file ClientDispatcher.hpp 
 * @brief Subclass of consumer, consumes from the currentNumbers queue and produces on the factorization queue
 * @author Team Espera Activa
 * @date 12/2021
 */

#ifndef CLIENT_DISPATCHER_
#define CLIENT_DISPATCHER_

#include <vector>

#include "Consumer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "NumbersStructure.hpp"

class ClientDispatcher : public Consumer<std::vector<Numbers>> {
 private:
  std::vector<Numbers> currentNumbers;
  /** 
   * @brief Method to print the numbers and its factors.
   * 
   * @param numbers 
   * @param status // valid 1 no valid 0 
   * @param factors 
   * @param httpResponse& httpResponse
   */
  void printNumbers(std::vector<int64_t>& numbers,
    std::vector<int64_t>& status, std::vector<std::vector<int64_t>>& factors,
    HttpResponse& httpResponse);

  /** 
   * @brief Method to print the end of the response.
   * 
   * @param httpResponse 
   * 
   */
  void printEnd(HttpResponse& httpResponse);

  /** 
   * @brief Method to print the title of the results page.
   * 
   * @param valid
   * @param httpResponse 
   * 
   */
    void printTitle(bool valid, HttpResponse& httpResponse);

  /** 
   * @brief Method aux to sort the numbers upward
   * 
   * @param first
   * @param second
   * 
   */  
  static bool smaller(Numbers first, Numbers second);

 protected:
   /** 
   * @brief Method to start consuming forever.
   */
  int run();

  /** 
   * @brief Method to consume from currentNumbers
   * 
   * @param currentNumbers
   * 
   */ 
    void consume(std::vector<Numbers> currentNumbers);

   /** 
   * @brief Method that consume from currentNumbers until the stop signal arrive.
   */
    void consumeForever();

 public:
   /** 
   * @brief Constructor by default
   */
    ClientDispatcher();

   /** 
   * @brief Destructor
   */
  ~ClientDispatcher();

     /** 
   * @brief Method to get Instance of the dispatcher
   */
  static ClientDispatcher& getInstance();
};

#endif  // CLIENT_DISPATCHER_
