// Copyright Espera Activa

/**
 * @file Packer.hpp 
 * @brief Subclass of assembler, consumes from the factorizations queue and produces on the dispatcher queue
 * @author Team Espera Activa
 * @date 12/2021
 */

#ifndef PACKER_
#define PACKER_

#include <vector>

#include "Assembler.hpp"
#include "NumbersStructure.hpp"

class Packer : public Assembler<Numbers, std::vector<Numbers>> {
 private:
  std::vector<std::vector<Numbers>> requests;  ///< structure in which the
  ///< packer produces, stores the factored numbers

  /**
   * @brief method that checks when the request vector is full
   * @param position position of the outer vector that indicates the start of the inner vector
   * @return bool true if the vector is full, false otherwise
   */
  bool full(size_t position);

 protected:
  /**
   * @brief invokes the method that consumes forever
   * @return returns 1 if consume forever runs successfully
   */
  int run();

  /**
   * @brief method that consumes from the factorizations queue and when it should, produces httpRequest in the requests vector
   * @param number struct representing the factored number
   */
  void consume(Numbers number);

  /**
   * @brief pop up the consumption queue and invoke the consume method
   */
  void consumeForever();

 public:
  /**
   * @brief Class destroyer, clear the consume queue.
   */
  ~Packer();

  /**
   * @brief Class constructor, initialize the production queue.
   */
  Packer();
  /**
   * @brief Method that returns the only packer instance existing in the current process
   * @return static Packer, the only packer instance existing in the current process
   */
  static Packer& getInstance();
};

#endif  // PACKER_
