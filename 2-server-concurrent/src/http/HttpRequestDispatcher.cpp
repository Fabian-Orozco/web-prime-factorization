// Copyright Espera Activa

#include <cassert>
#include <iostream>

#include "FactWebApp.hpp"
#include "Socket.hpp"
#include "HttpRequestDispatcher.hpp"

HttpRequestDispatcher::HttpRequestDispatcher() {
  this->createOwnQueue();
}

HttpRequestDispatcher::~HttpRequestDispatcher() {
  if (this->consumingQueue) {
    delete this->consumingQueue;
    this->consumingQueue = nullptr;
  }
}

HttpRequestDispatcher& HttpRequestDispatcher::getInstance() {
  static HttpRequestDispatcher dispatcher;
  return dispatcher;
}

std::string HttpRequestDispatcher::extractKey(const HttpReqRes& data) const {
  std::string uri = data.request.getURI();
  std::string returnValue;
  int position = 0;

  position = uri.find("/")+1;

  // scroll down the row until you find character "/" or "?"
  for (; uri[position] != '/' && uri[position] != '?'; ++position) {
    returnValue += uri[position];
  }
  return returnValue;
}

int HttpRequestDispatcher::run() {
  this->consumeForever();
  return 1;
}

void HttpRequestDispatcher::consume(HttpReqRes data) {
  // call extractKey
  const std::string& key = this->extractKey(data);
  // produce HttpRequest into requestQueue that matches key
  const auto& itr = this->toQueues.find(key);
  if ( itr != this->toQueues.end() ) {
    itr->second->push(data);
  } else {
    std::cerr << "dispatcher: queue's key not found" << std::endl;
  }
}
