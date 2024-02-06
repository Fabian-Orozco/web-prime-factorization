// Copyright EsperaActiva

/**
 * @file HttpReqRes.hpp 
 * @brief Struct containing the request and response of a socket 
 * @author Team Espera Activa
 * @date 12/2021
 */

#ifndef HTTP_REQ_RES_
#define HTTP_REQ_RES_

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Socket.hpp"

struct HttpReqRes {
  /**
   * @brief Default constructor
   * @details list-initialization with response and request of a object in constructor
   */
  HttpReqRes() : response(socket),
    request(socket) {}

  /**
   * @brief Constructor with parameters 
   * @details list-initialization with response and request of a object in constructor
   * @param socket with which the request, response are initialized
   */
  explicit HttpReqRes(const Socket& socket) : response(socket),
    request(socket) {
    this->socket = socket;
  }

  Socket socket;  ///< socket attribute bound to a single client
  HttpResponse response;  ///< response attribute bound to a single client
  HttpRequest request;  ///< request attribute bound to a single client

  /**
   * @brief overloaded equality operator
   * @param other HttpReqRest object to which it wants to be compared
   * @return 1 if the objects are equal
   * @return 0 otherwise
   */
  int operator==(const HttpReqRes& other) const {
    return this->socket  == other.socket;
  }
};

#endif  // HTTP_REQ_RES_
