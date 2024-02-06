// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#define INVALID_REQUEST -3

#include <unistd.h>
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
#include "Packer.hpp"

FactWebApp::FactWebApp() {
  this->key = "fact";
  this->currentRequest = 0;
  this->numbersQueue = new Queue<Numbers>();
  this->createOwnQueue();
}

FactWebApp::~FactWebApp() {
  if (this->consumingQueue) {
    delete this->consumingQueue;
    this->consumingQueue = nullptr;
  }
  if (this->numbersQueue) {
    delete this->numbersQueue;
    this->numbersQueue = nullptr;
  }
  for (size_t currentFactorizer = 0; currentFactorizer < factorizers.size();
    ++currentFactorizer) {
    delete factorizers[currentFactorizer];
  }
}

void FactWebApp::start() {
  uint64_t factorizer_count = sysconf(_SC_NPROCESSORS_ONLN);
  for (uint64_t currentFactorizer = 0; currentFactorizer < factorizer_count;
    ++currentFactorizer) {
    Factorizer* add = new Factorizer();
    add->setConsumingQueue(this->numbersQueue);
    add->setProducingQueue(this->packer.getConsumingQueue());
    this->factorizers.push_back(add);
  }
  this->dispatcher.setConsumingQueue(this->packer.getProducingQueue());
  this->packer.startThread();
  this->dispatcher.startThread();

  for (uint64_t currentFactorizer = 0; currentFactorizer < factorizer_count;
    ++currentFactorizer) {
    this->factorizers[currentFactorizer]->startThread();
  }
}

void FactWebApp::stop() {
  HttpReqRes stop;
  this->getConsumingQueue()->push(stop);
  Numbers stopNumber;
  stopNumber.numberIndex = -1;
  for (uint64_t currentFactorizer = 0; currentFactorizer < factorizers.size();
    ++currentFactorizer) {
    this->factorizers[currentFactorizer]->getConsumingQueue()->push(stopNumber);
  }
  this->packer.getConsumingQueue()->push(stopNumber);
  this->packer.waitToFinish();
  std::vector<Numbers> stopNumbers;
  stopNumbers.push_back(stopNumber);
  this->dispatcher.getConsumingQueue()->push(stopNumbers);
  this->dispatcher.waitToFinish();

  for (uint64_t currentFactorizer = 0; currentFactorizer < factorizers.size();
    ++currentFactorizer) {
    this->factorizers[currentFactorizer]->waitToFinish();
  }
  this->waitToFinish();
}

bool FactWebApp::handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) {
  // If the home page was asked
  if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/") {
    return this->serveHomepage(httpRequest, httpResponse);
  }
  // If the request starts with "fact/" is for this web app
  if (httpRequest.getURI().rfind("/fact", 0) == 0) {
    return true;
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

int FactWebApp::run() {
  this->consumeForever();
  return 1;
}

/**
 * @brief This method adds all the numbers in the httpRequest queue
 * 
 * @param request HttpReqRes contains the client request
 */
void FactWebApp::consume(HttpReqRes request) {
  // If a number was asked in the form "/fact/1223"
  // or "/fact?number=1223"
  // TODO(you): Use arbitrary precision for numbers larger than int64_t
  std::string numberLine = this->getNumbers(request.request.getURI());
  Numbers currentNumber;
  if (numberLine != "") {
    numberManager.extractNumbers(numberLine);
    std::vector<int64_t> allNumbers = numberManager.getHttpNumbers();
    numberManager.numbers.clear();
    allNumbers.shrink_to_fit();
    std::vector<int64_t> invalids = numberManager.getHttpStatus();
    numberManager.status.clear();
    invalids.shrink_to_fit();


    for (size_t index = 0; index < allNumbers.size(); ++index) {
      currentNumber.number = allNumbers[index];
      std::cout << "Numero leido: " << allNumbers[index] <<std::endl;
      currentNumber.numbersCount = allNumbers.size();
      currentNumber.status = invalids[index];
      currentNumber.requestNumber = currentRequest;
      currentNumber.numberIndex = index;
      currentNumber.myRequest = request;
      this->numbersQueue->push(currentNumber);
    }
    ++currentRequest;
  } else {
    currentNumber.number = INVALID_REQUEST;
    currentNumber.numbersCount = INVALID_REQUEST;
    currentNumber.status = INVALID_REQUEST;
    currentNumber.requestNumber = INVALID_REQUEST;
    currentNumber.numberIndex = INVALID_REQUEST;
    currentNumber.myRequest = request;
    this->numbersQueue->push(currentNumber);
    ++currentRequest;
  }
}
