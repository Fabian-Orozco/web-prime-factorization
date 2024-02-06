// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <vector>
#include "HttpMessage.hpp"


class Socket;

class HttpRequest : public HttpMessage {
 private:
/**
 * @brief This method takes the input and separates the numbers from the comas
 * ans adds them to a vector of strings
 * 
 * @param line 
 * @param counter 
 * @param words 
 * @param sem 
 */
  void separateNumbers(std::string& line, std::size_t& counter,
    std::vector<std::string>& words, bool& sem);

/**
 * @brief Takes the vector of strings created previously to convert all
 * the numbers to int64 to be factorized.
 * 
 * @param counter 
 * @param words 
 * @param sem 
 * @param aux 
 * @param limite 
 */
  void setNumbers(std::size_t& counter, std::vector<std::string>& words,
    bool& sem, int64_t aux, int64_t limite);

 public:
  /// Objects of this class can be copied, but avoid innecesary copies
  DECLARE_RULE4(HttpRequest, default);

 protected:
  /// HTTP method: GET, POST, HEADERS, DELETE, UPDATE, ...
  std::string method;
  /// URI /path?query=string#fragment
  std::string uri;
  std::vector<int64_t> numbers;  /// numbers for factorization.
  std::vector<int64_t> status;  /// valid or not valid of those numbers.

 public:
  /// Constructor
  explicit HttpRequest(const Socket& socket);
  /// Destructor
  ~HttpRequest();
  /// Parses an HTTP request from the data sent from the socket
  /// @return true on success, false on error or connection closed by peer
  bool parse();
  /// Get access to the HTTP method used by client
  inline const std::string& getMethod() const { return this->method; }
  /// Get access to the resource address (URI) asked by client
  inline const std::string& getURI() const { return this->uri; }
  /// Get access to the HTTP version used by client
  inline const std::string& getHttpVersion() const { return this->httpVersion; }
  /// Get acces to the numbers vector.
  inline const std::vector<int64_t>& getHttpNumbers()  { return this->numbers; }
  /// Get acces to the status vector.
  inline const std::vector<int64_t>& getHttpStatus()  { return this->status; }

  /**
   * @brief This methos takes a string in the form 1,32,4,6,5,41 and separates
   * each number from the strng and adds it to the status vector and numbers
   * vector respectively.
   * 
   * @param numbersString
   */
  void extractNumbers(std::string numbersString);

 protected:
  /// Parse the request line from the socket
  /// @return true on success, false on error or connection closed by peer
  bool parseRequestLine();
  /// Parse the message header, containing pairs "key: value" of metadata
  /// @return true on success, false on error or connection closed by peer
  bool parseHeader();
  /// Parse the optional message body content, if any
  /// @return true on success, false on error or connection closed by peer
  bool parseBody();
};

#endif  // HTTPREQUEST_H
