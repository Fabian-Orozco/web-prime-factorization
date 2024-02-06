// Copyright [2021] esperaActiva

/**
 * @file HttpConnectionHandler.hpp 
 * @brief Consumer subclass that takes requests from clients and queues them to the socket queue 
 * @author Team Espera Activa
 * @date 12/2021
 */

#ifndef HTTP_CONNECTION_HDLR_
#define HTTP_CONNECTION_HDLR_

#include <vector>

#include "Consumer.hpp"
#include "HttpApp.hpp"
#include "HttpReqRes.hpp"
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
  */ 
  HttpConnectionHandler();

  /**
   * @brief Destroyer of HttpConnectionHandler
   * @details add a sentinel to the consumption queue to stop it (nullptr)
   */ 
  ~HttpConnectionHandler();

  /**
  * @brief Method to run the HttpConnectionHandler, all it does is consume
  * from the buffer for ever.
  * @return int 1
  */   
  int run();

  /**
  * @brief Method to handle the request from the server.
  * @param httpRequest& object with request
  * @param httpResponse& object in which the response will be stored 
  * @return bool false or true depending on whether the path between the httpRequest and httpResponse could be set
  */
  bool handleHttpRequest(HttpRequest& httpRequest,
  HttpResponse& httpResponse);

  /**
  * @brief Method to call the handle request of all apps to see if they can take the request
  * @param httpRequest& object with request
  * @param httpResponse& object in which the response will be stored
  * @return false or true depending on whether a suitable application could be found for the request
  */
  bool route(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /**
   * @brief Method that prints a serve not found to the screen in case the app is not available
   * @param httpRequest& object with request
   * @param httpResponse& object in which the response will be stored 
   * @return true 
   */
  bool serveNotFound(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /**
   * @brief Consume method that takes the socket and creats a httpRequest from it and if it is handled then we push it to the request queue
   * @param client client to generate a copy of the request
   */
  void consume(Socket client);

  /**
   * @brief method to set the applications queue pointer
   * @param applications vector of applications linked to the server 
   */
  void setApplicationQueue(std::vector<HttpApp*>* applications);

  Queue<HttpReqRes>* requestQueue;  ///< pointer to HttpRequestDispatcher
  ///< consuming queue
};

#endif  // HTTP_CONNECTION_HDLR_
