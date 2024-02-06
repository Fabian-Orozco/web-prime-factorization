// Copyright Espera Activa

/**
 * @file HttpRequestDispatcher.hpp 
 * @brief Subclass of dispatcher, consumes from the httpReqRes queue and produces on the requestHttp_app queue
 * @author Team Espera Activa
 * @date 12/2021
 */

#ifndef HTTP_REQUEST_DISPATCHER_
#define HTTP_REQUEST_DISPATCHER_

#include <string>

#include "Dispatcher.hpp"
#include "HttpApp.hpp"
#include "HttpReqRes.hpp"
#include "HttpRequest.hpp"

class HttpRequestDispatcher : public Dispatcher<std::string, HttpReqRes> {
 private:
  /**
   * @brief Construct a new Http Request Dispatcher
   * @details creates it's consuming queue
   */
  HttpRequestDispatcher();

  /**
   * @brief Destroy the Http Request Dispatcher
   * @details it only destroys it's consuming queue
   */
  ~HttpRequestDispatcher();

 public:
  /**
   * @brief Returns an instance of the Request dispatcher
   * @details There can only be one dispatcher in the whole program.
   * @return HttpRequestDispatcher& instance
   */
  static HttpRequestDispatcher& getInstance();

  /**
   * @brief Method to extract the type of app that corresponds to the request
   * @param data HttpReqRes which contains the uri of the request
   * @return std::string representing the key
   */
  std::string extractKey(const HttpReqRes& data) const;

  /**
   * @brief Every consumer has a run method that calls the method consume forever
   * @return int 1
   */
  int run();

  /**
   * @brief Consume method fot the Http Request Dispatcher
   * @details it takes a key and adds the request to the corresponding queue.
   * 
   * @param data HttpReqRes from which the key will be extracted and then processed
   */
  void consume(HttpReqRes data);

  Queue<HttpReqRes>* requestQueue;  ///< queue containing requests and responses
};

#endif  // HTTP_REQUEST_DISPATCHER_
