// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef FACTWEBAPP_HPP
#define FACTWEBAPP_HPP

#include <string>
#include <vector>

#include "HttpApp.hpp"
#include "mathmatician.hpp"

/**
@brief A web application that calculates prime factors
*/
class FactWebApp : public HttpApp {
  /// Objects of this class cannot be copied
  DISABLE_COPY(FactWebApp);

 private:
  /**
   * @brief Method to call the method to factorize all numbers
   * 
   * @param factors 
   * @param mathmatician 
   * @param numbers 
   */  void calculate_factorization(std::vector<std::vector<int64_t>>& factors,
    Mathmatician& mathmatician, std::vector<int64_t>& numbers);

  /// Method to print the end of the response
  void printEnd(HttpResponse& httpResponse);

  /// Method to print the title of the results page
  void printTitle(bool valid, HttpResponse& httpResponse);

 public:
  /// Constructor
  FactWebApp();
  /// Destructor
  ~FactWebApp();
  /// Called by the web server when the web server is started
  void start() override;
  /// Handle HTTP requests. @see HttpServer::handleHttpRequest()
  /// @return true If this application handled the request, false otherwise
  /// and another chained application should handle it
  bool handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) override;
  /// Called when the web server stops, in order to allow the web application
  /// clean up and finish as well
  void stop() override;

  /**
   * @brief This method takes the URI and separates the string of numbers
   * from the URI string.
   * 
   * @param URI 
   * @return std::string 
   */
  std::string getNumbers(const std::string & URI);

  /**
   * @brief Method print the factorization of the numbers given by the user.
   * 
   * @param std::vector<int64_t>& numbers
   * @param std::vector<int64_t>& status
   * @param std::vector<std::vector<int64_t>>& factors
   * @param HttpResponse& httpResponse
   * @return std::string 
   */
  void printNumbers(std::vector<int64_t>& numbers,
    std::vector<int64_t>& status, std::vector<std::vector<int64_t>>& factors,
    HttpResponse& httpResponse);

 protected:
  /// Handle HTTP requests. @see HttpServer::handleHttpRequest()
  /// @return true If this application handled the request, false otherwise
  /// Sends the homepage as HTTP response
  bool serveHomepage(HttpRequest& httpRequest, HttpResponse& httpResponse);
  /// Handle a HTTP request that starts with "/fact"
  /// @return true if the factorization was handled, false if it must be
  /// handled by another application
  bool serveFactorization(HttpRequest& httpRequest, HttpResponse& httpResponse);
};

#endif  // FACTWEBAPP_HPP
