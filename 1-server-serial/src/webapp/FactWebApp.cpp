// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#define VALID_NUMBER 1
#define INVALID_NUMBER 0
#define NA_NUMBER -1
#define NEGATIVE_NUMBER -2

#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

#include "FactWebApp.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "mathmatician.hpp"

FactWebApp::FactWebApp() {
}

FactWebApp::~FactWebApp() {
}

void FactWebApp::start() {
  // TODO(you): Start producers, consumers, assemblers...
}

void FactWebApp::stop() {
  // TODO(you): Stop producers, consumers, assemblers...
}

bool FactWebApp::handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) {
  // If the home page was asked
  if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/") {
    return this->serveHomepage(httpRequest, httpResponse);
  }

  // If the request starts with "fact/" is for this web app
  if (httpRequest.getURI().rfind("/fact", 0) == 0) {
    return this->serveFactorization(httpRequest, httpResponse);
  }

  // Unrecognized request
  return false;
}

// TODO(you): Fix code redundancy in the following methods

bool FactWebApp::serveHomepage(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

  // Set HTTP response metadata (headers)
  httpResponse.setHeader("Server", "AttoServer v1.1");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  std::string title = "Prime factorization";
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace}</style>\n"
    << "  <h1>" << title << "</h1>\n"
    << "  <form method=\"get\" action=\"/fact\">\n"
    << "    <label for=\"number\">Number</label>\n"
    << "    <input type=\"string\" name=\"number\" required/>\n"
    << "    <button type=\"submit\">Factorize</button>\n"
    << "  </form>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}

/**
 * @brief This method reads the input, separates the numbers in vectors
 * and calculates the factorization of each number.
 * 
 * @param httpRequest 
 * @param httpResponse 
 * @return true 
 * @return false 
 */
bool FactWebApp::serveFactorization(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

  // Set HTTP response metadata (headers)
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // If a number was asked in the form "/fact/1223"
  // or "/fact?number=1223"
  // TODO(you): Use arbitrary precision for numbers larger than int64_t
  // TODO(you): Modularize this method
  std::string numberLine = this->getNumbers(httpRequest.getURI());
  if (numberLine != "") {
    httpRequest.extractNumbers(numberLine);
    Mathmatician mathmatician;

    std::vector<int64_t> numbers = httpRequest.getHttpNumbers();
    numbers.shrink_to_fit();
    std::vector<int64_t> status = httpRequest.getHttpStatus();
    status.shrink_to_fit();
    std::vector<std::vector<int64_t>> factors(numbers.size());

    calculate_factorization(factors, mathmatician, numbers);

    // TODO(you): Factorization must not be done by factorization threads
    // Build the body of the response
    this->printTitle(1, httpResponse);
    this->printNumbers(numbers, status, factors, httpResponse);
    this->printEnd(httpResponse);
  } else {
    this->printTitle(0, httpResponse);
    this->printEnd(httpResponse);
  }

  // Send the response to the client (user agent)
  return httpResponse.send();
}

void FactWebApp::calculate_factorization(std::vector<std::vector<int64_t>>&
  factors, Mathmatician& mathmatician, std::vector<int64_t>& numbers) {
  for (size_t factors_rows = 0; factors_rows < numbers.size()
    ; ++factors_rows) {
    mathmatician.calculate_prime_factors(numbers[factors_rows], factors,
      factors_rows);
  }

  for (size_t factors_rows = 0; factors_rows < numbers.size();
    ++factors_rows) {
    factors[factors_rows].shrink_to_fit();
  }
}

/**
 * @brief Method to print the end of the results page
 * @details includes a back button and a line separating the factorized numbers
 * 
 * @param httpResponse 
 */
void FactWebApp::printEnd(HttpResponse& httpResponse) {
  httpResponse.body()
      << "  <hr><p><a href=\"/\">Back</a></p>\n"
      << "</html>\n";
}

/**
 * @brief Method to print the title of the results page
 * @details receives a bool to know if the title corresponds to a
 * valid or invalid number
 * 
 * @param valid 
 * @param httpResponse 
 */
void FactWebApp::printTitle(bool valid, HttpResponse& httpResponse) {
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

std::string FactWebApp::getNumbers(const std::string & URI) {
  std::string line = "";
  const std::string nameApp = "/fact?number=";
  size_t position = URI.find(nameApp);

  if (position != std::string::npos) {
    position += nameApp.size();
    line = URI.substr(position);
  } else {
    std::smatch matches;
    std::regex regexLine("[^/]*$");
    if (std::regex_search(URI, matches, regexLine)) {
      line = matches[0];
    }
  }
  return line;
}

void FactWebApp::printNumbers(std::vector<int64_t>& numbers,
    std::vector<int64_t>& status, std::vector<std::vector<int64_t>>& factors,
    HttpResponse& httpResponse) {
  for (size_t current_value_index = 0; current_value_index < numbers.size();
    ++current_value_index) {
    if (status[current_value_index] == INVALID_NUMBER) {
      httpResponse.body() << "  <h2 class=\"err\"> Number in position "
        << current_value_index+1 << "</h2>\n"
        << "  <p>invalid number</p>\n";
    } else {
      int64_t current_number = numbers[current_value_index];
      httpResponse.body() << "  <h2>" << current_number << "</h2>\n"
        <<  "  <p>" << current_number;
      for (size_t current_factors_index = 0;
          current_factors_index < factors[current_value_index].size();
            ++current_factors_index) {
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
