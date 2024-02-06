// Copyright [2021] esperaActiva

#include <string>
#include "HttpApp.hpp"
#include "HttpConnectionHandler.hpp"
#include "HttpServer.hpp"
#include "HttpReqRes.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "Socket.hpp"


HttpConnectionHandler::HttpConnectionHandler() {
}

HttpConnectionHandler::~HttpConnectionHandler() {
  this->setConsumingQueue(nullptr);
}

int HttpConnectionHandler::run() {
  consumeForever();
  return 1;
}

bool HttpConnectionHandler::handleHttpRequest(HttpRequest& httpRequest,
  HttpResponse& httpResponse) {
  // Print IP and port from client
  const NetworkAddress& address = httpRequest.getNetworkAddress();
  Log::append(Log::INFO, "connection",
    std::string("connection established with client ") + address.getIP()
    + " port " + std::to_string(address.getPort()));

  // Print HTTP request
  Log::append(Log::INFO, "request", httpRequest.getMethod()
    + ' ' + httpRequest.getURI()
    + ' ' + httpRequest.getHttpVersion());

  return this->route(httpRequest, httpResponse);
}

bool HttpConnectionHandler::route(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  // Traverse the chain of applications
  for (size_t index = 0; index < this->applications->size(); ++index) {
    // If this application handles the request
    HttpApp* app = (*this->applications)[index];
    if (app->handleHttpRequest(httpRequest, httpResponse)) {
      return true;
    }
  }

  // Unrecognized request
  return this->serveNotFound(httpRequest, httpResponse);
}

bool HttpConnectionHandler::serveNotFound(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

  // Set HTTP response metadata (headers)
  httpResponse.setStatusCode(404);
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  std::string title = "Not found";
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace} h1 {color: red}</style>\n"
    << "  <h1>" << title << "</h1>\n"
    << "  <p>The requested resouce was not found on this server.</p>\n"
    << "  <hr><p><a href=\"/\">Homepage</a></p>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}

void HttpConnectionHandler::consume(Socket client) {
  while (true) {
    // Create an object that parses the HTTP request from the socket
    // A complete HTTP client request was received. Create an object for the
    // server responds to that client's request
    HttpReqRes request(client);
    std::cout << request.request.getURI() << std::endl;

    // If the request is not valid or an error happened
    if (request.request.parse()) {
      // Give subclass a chance to respond the HTTP request
      const bool handled = this->handleHttpRequest(request.request,
        request.response);

      // If subclass did not handle the request or the client used HTTP/1.0
      if (!handled || request.request.getHttpVersion() == "HTTP/1.0") {
        // The socket will not be more used, close the connection
        client.close();
        break;
      } else {
        this->requestQueue->push(request);
      }
    } else {
      break;
    }
  }
}

void HttpConnectionHandler::setApplicationQueue
  (std::vector<HttpApp*>* applications) {
  this->applications = applications;
}
