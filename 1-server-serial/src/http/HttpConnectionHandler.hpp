// Copyright [2021] esperaActiva

#ifndef HTTP_CONNECTION_HDLR_
#define HTTP_CONNECTION_HDLR_

#include <vector>

#include "Consumer.hpp"
#include "HttpApp.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Socket.hpp"

class HttpConnectionHandler : public Consumer<Socket> {
 protected:
  std::vector<HttpApp*>* applications;

 public:
  /**
  * @brief Construct a new Http Connection 
  * Handler:: Http Connection Handler object, does nothing.
  * 
  */ 
  HttpConnectionHandler();

  /**
  * @brief Method to run the HttpConnectionHandler, all it does is consume
  * from the buffer for ever.
  * 
  * @return int
  */   
  int run();

  /**
  * @brief Method to handle the request from the server.
  * 
  * @param HttpRequest& httpRequest
  * @param HttpResponse& httpResponse
  * @return bool 
  */
  bool handleHttpRequest(HttpRequest& httpRequest,
  HttpResponse& httpResponse);

  /**
  * @brief Route, that provide an answer according to the URI value.
  * For example, home page is handled different than a number
  * @param HttpRequest& httpRequest
  * @param HttpResponse& httpResponse
  * @return bool 
  */
  bool route(HttpRequest& httpRequest, HttpResponse& httpResponse);

  // Sends a page for a non found resouce in this server.
  // This method is called
  // if none of the registered web applications handled the request.
  // If you want to override this method, create a web app, e.g NotFoundWebApp
  // that reacts to all URIs, and chain it as the last web app
  bool serveNotFound(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /**
  * @brief Method in charge of create an object that parses the HTTP request from the socket.
  * Create an object for the server responds to that client's request.
  * Give subclass a chance to respond the HTTP request
  * 
  * @param Socket client
  */
  void consume(Socket client);

  /**
  * @brief Sets the vector to each application the server offers.
  * 
  * @param applications 
  */
  void setApplicationQueue(std::vector<HttpApp*>* applications);
};

#endif  // HTTP_CONNECTION_HDLR_
