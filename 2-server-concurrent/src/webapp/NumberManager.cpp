// Copyright Espera Activa
#define VALID_NUMBER 1
#define INVALID_NUMBER 0

#include "NumberManager.hpp"

/// Method to call separate the Numbers in the string given.
void NumberManager::extractNumbers(std::string numbersString) {
  std::string line = numbersString;
  std::size_t counter = 0;
  std::vector<std::string> words;  /// vector to store the words divided by ','
  int64_t aux = 0;
  int64_t limite = 9223372036854775807;
  bool sem = false;

  separateNumbers(line, counter, words, sem);

  counter = 0;
  /// Method to set the numbers in the vector of int64_t, and to sort the valid
  /// from the invalid ones.
  setNumbers(counter, words, sem, aux, limite);
}

/// Method that finds for what are the numbers
/// divided ( , or ASCII or theres only one number)
/// and split the string int those divisions.
void NumberManager::separateNumbers(std::string& line, std::size_t& counter,
  std::vector<std::string>& words, bool& sem) {
  if (line.find(',') != std::string::npos&&!sem) {
    /// If they are divided by ','
    sem = true;
    while ((counter = line.find(',')) != std::string::npos) {
      /// While there's another , take to number from the position 0 to
      /// the position of , and place it in the words vector.
      /// Then, erase those chars, to continue with the next number.
      words.push_back(line.substr(0, counter));
      line.erase(0, counter+1);
    }
    /// Push the last couple of chars.
    words.push_back(line);
  } else {
    if (line.find("%2C") != std::string::npos&&!sem) {
      /// If they are divided by '%2C', this option happens usually when the
      /// user makes an URI request.

      sem = true;
      while ((counter = line.find("%2C")) != std::string::npos) {
        words.push_back(line.substr(0, counter));
        line.erase(0, counter+3);
      }
      words.push_back(line);
    } else {
      /// If it is only one number.
      words.push_back(line);
    }
  }
}

/// This method convert the strings to a valid number, using c_str()
/// And if its a valid number then pushes 1 in the status vector
/// or 0 if it isnt.
void NumberManager::setNumbers(std::size_t& counter,
  std::vector<std::string>& words, bool& sem, int64_t aux, int64_t limite) {
  if (sem) {
    while (counter < words.size()) {
      try {
        sscanf(words[counter].c_str() , "%" PRId64 , &aux);
        numbers.push_back(aux);
        if (-limite < aux && aux <= limite && errno == 0) {
          status.push_back(VALID_NUMBER);
        } else {
          status.push_back(INVALID_NUMBER);
          errno = 0;
        }
      } catch (const std::out_of_range& err) {
        numbers.push_back(1);
        status.push_back(INVALID_NUMBER);
      }
      ++counter;
    }
  } else {
      if (sscanf(words[counter].c_str() , "%" PRId64 , &aux) == 1) {
      numbers.push_back(aux);
    if (-limite < aux && aux <= limite && errno == 0) {
        status.push_back(VALID_NUMBER);
    } else {
        status.push_back(INVALID_NUMBER);
        errno = 0;
      }
    }
  }
}
