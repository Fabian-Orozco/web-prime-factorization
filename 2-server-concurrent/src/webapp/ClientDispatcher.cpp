// Copyright Espera Activa
#define VALID_NUMBER 1
#define INVALID_NUMBER 0
#define NA_NUMBER -1
#define NEGATIVE_NUMBER -2
#define INVALID_REQUEST -3

#include <algorithm>
#include <string>

#include "ClientDispatcher.hpp"
#include "FactWebApp.hpp"
#include "HttpResponse.hpp"
#include "NumbersStructure.hpp"

/// Constructor by default.
ClientDispatcher::ClientDispatcher() {
}

/// Destructor.
ClientDispatcher::~ClientDispatcher() {
  if (this->consumingQueue) {
    delete this->consumingQueue;
  }
}

/// Allows to get the Instance of the dispatcher.
ClientDispatcher& ClientDispatcher::getInstance() {
  static ClientDispatcher clientDispatcher;
  return clientDispatcher;
}

/// Method to start consuming in the queue forever.
int ClientDispatcher::run() {
  this->consumeForever();
  return 1;
}

/// Method that consume from currentNumbers queue until the stop signal arrive.
void ClientDispatcher::consumeForever() {
  assert(this->consumingQueue);
  while (true) {
    // Get the next data to consume, or block while queue is empty
    const std::vector<Numbers>& data = this->consumingQueue->pop();
    // If data is the stop condition, stop the loop
    if ( data[0].numberIndex == -1 ) {
      break;
    }
    // Process this data
    this->consume(data);
  }
}

/// Method to get the smaller number of two given.
bool ClientDispatcher::smaller(Numbers first, Numbers second) {
  return (first.numberIndex < second.numberIndex);
}

/// Method to consume from currentNumbers queue.
void ClientDispatcher::consume(std::vector<Numbers>
  currentNumbers) {
  if (currentNumbers[0].numberIndex != INVALID_REQUEST) {
    /// Verifies that is a valid request.
    std::vector<int64_t> numbers;  ///< vector of the numbers to factorize.
    std::vector<int64_t> status;  ///< vector with the status of every number.
    std::vector<std::vector<int64_t>> factors;  ///< vector of vector of int64_t
    /// to store de factorizations of the numbers.
    std::sort(currentNumbers.begin(), currentNumbers.end(), smaller);
    for (size_t index = 0; index < currentNumbers.size(); ++index) {
      factors.push_back(currentNumbers[index].factorization);
      numbers.push_back(currentNumbers[index].number);
      status.push_back(currentNumbers[index].status);
    }
    // Set HTTP response metadata (headers)
    currentNumbers[0].myRequest.response.setHeader("Server", "AttoServer v1.0");
    currentNumbers[0].myRequest.response.setHeader("Content-type",
      "text/html; charset=ascii");
    this->printTitle(1, currentNumbers[0].myRequest.response);
    this->printNumbers(numbers, status, factors,
      currentNumbers[0].myRequest.response);
    this->printEnd(currentNumbers[0].myRequest.response);
  } else {
    // Set HTTP response metadata (headers)
    currentNumbers[0].myRequest.response.setHeader("Server", "AttoServer v1.0");
    currentNumbers[0].myRequest.response.setHeader("Content-type",
      "text/html; charset=ascii");
    this->printTitle(0, currentNumbers[0].myRequest.response);
    this->printEnd(currentNumbers[0].myRequest.response);
  }
  /// Answer request.
  currentNumbers[0].myRequest.response.send();
}

/// Method to print the end of the response.
void ClientDispatcher::printEnd(HttpResponse& httpResponse) {
  httpResponse.body()
    << "  <hr><p><a href=\"/\">Back</a></p>\n"
    << "</html>\n";
}

/// Method to print the title of the results page.
void ClientDispatcher::printTitle(bool valid, HttpResponse& httpResponse) {
  if (valid) {
    // Build body for valid request
    std::string title = "Prime factorization";
    httpResponse.body() << "<!DOCTYPE html>\n"
      << "<html lang=\"en\">\n"
      << "  <meta charset=\"ascii\"/>\n"
      << "  <title>" << title << "</title>\n"
      << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
      << "  <h1>" << title << "</h1>\n";
  } else {
    // Build the body for an invalid request
    std::string title = "Invalid request";
    httpResponse.body() << "<!DOCTYPE html>\n"
      << "<html lang=\"en\">\n"
      << "  <meta charset=\"ascii\"/>\n"
      << "  <title>" << title << "</title>\n"
      << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
      << "  <h1 class=\"err\">" << title << "</h1>\n"
      << "  <p>Invalid request for factorization</p>\n";
  }
}

/// Method that allows to print the numbers with its factorization.
void ClientDispatcher::printNumbers(std::vector<int64_t>& numbers,
  std::vector<int64_t>& status, std::vector<std::vector<int64_t>>& factors,
  HttpResponse& httpResponse) {
  for (size_t current_value_index = 0; current_value_index < numbers.size();
    ++current_value_index) {
    /// For every number received prints its information.
    if (status[current_value_index] == INVALID_NUMBER) {
      /// Response if the number is invalid.
      httpResponse.body() << "  <h2 class=\"err\"> Number in position "
        << current_value_index+1 << "</h2>\n"
        << "  <p>invalid number</p>\n";
    } else {
      /// Response if the number is valid.
      int64_t current_number = numbers[current_value_index];
      httpResponse.body() << "  <h2>" << current_number << "</h2>\n"
        <<  "  <p>" << current_number;
      for (size_t current_factors_index = 0;
          current_factors_index < factors[current_value_index].size();
            ++current_factors_index) {
              /// print its factors.
        if (factors[current_value_index][
          current_factors_index] == NA_NUMBER) {
          httpResponse.body() << ": NA</p>\n";
        } else if (factors[current_value_index][
          current_factors_index] == NEGATIVE_NUMBER) {
          httpResponse.body() << ": invalid number</p>\n";
        } else {
          if (current_factors_index == 0) {
            httpResponse.body() << " = ";
          }
          httpResponse.body()
            << factors[current_value_index][
          current_factors_index];
          if (factors[current_value_index][
          current_factors_index+1] != 1) {
            httpResponse.body() << "<sup>"
            << factors[current_value_index][
            current_factors_index+1] << "</sup>";
          }
        }
        if (current_factors_index+2 < factors[current_value_index].size()) {
          httpResponse.body() << " ";
        }
        current_factors_index += 1;
      }
    }
    httpResponse.body() << "\n";
  }
}
