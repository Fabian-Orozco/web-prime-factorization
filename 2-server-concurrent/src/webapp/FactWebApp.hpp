// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef FACTWEBAPP_HPP
#define FACTWEBAPP_HPP

#include <string>
#include <vector>

#include "NumberManager.hpp"
#include "ClientDispatcher.hpp"
#include "Factorizer.hpp"
#include "HttpApp.hpp"
#include "HttpRequest.hpp"
#include "Packer.hpp"
#include "Socket.hpp"

class HttpRequest;

/**
@brief A web application that calculates prime factors
*/
class FactWebApp : public HttpApp {
  /// Objects of this class cannot be copied
  DISABLE_COPY(FactWebApp);

 private:
  /// Vector of all factorizers
  std::vector<Factorizer*> factorizers;
  Packer packer;
  ClientDispatcher dispatcher;
  NumberManager numberManager;

 public:
  /// Constructor
  FactWebApp();
  /// Destructor
  ~FactWebApp();
  /// The queue for all numbers
  Queue<Numbers>* numbersQueue;

  /// variable to sabe all the requests.
  int currentRequest;
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

  /// variable to save the apps key
  std::string key;

  // gets the numbers from the user's input
  std::string getNumbers(const std::string & URI);
  /// Inherited method
  int run();
  /// inherited method
  void consume(HttpReqRes request);


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
