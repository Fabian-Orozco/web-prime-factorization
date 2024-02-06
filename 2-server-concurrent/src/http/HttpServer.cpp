// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <unistd.h>
#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

#include "HttpApp.hpp"
#include "HttpConnectionHandler.hpp"
#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestDispatcher.hpp"
#include "HttpResponse.hpp"
#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "Socket.hpp"
#include "Thread.hpp"

// TODO(you): Implement connection handlers argument
const char* const usage =
  "Usage: webserv [port] [handlers]\n"
  "\n"
  "  port        Network port to listen incoming HTTP requests, default "
    DEFAULT_PORT "\n"
  "  handlers     Number of connection handler theads\n";

/**
 * @brief Construct a new Http Server
 * @details just creates the sockets queue
 * 
 */
HttpServer::HttpServer() {
  this->clients = new Queue<Socket>();
}

HttpServer::~HttpServer() {
  delete clients;
}

HttpServer& HttpServer::getInstance() {
  static HttpServer httpServer;
  return httpServer;
}

void HttpServer::listenForever(const char* port) {
  return TcpServer::listenForever(port);
}

void HttpServer::handleClientConnection(Socket& client) {
  // TODO(you): Make this method concurrent. Store client connections (sockets)
  // into a collection (e.g thread-safe queue) and stop

  this->clients->push(client);
}

/**
 * @brief Adds a application to the application queue and registers the request
 * queue for it.
 * 
 * @param application 
 */
void HttpServer::chainWebApp(HttpApp* application) {
  assert(application);
  this->httpRequestDispatcher.registerRedirect("fact",
    application->getConsumingQueue());
  this->applications.push_back(application);
}

/**
 * @brief Method to initialize all connection handlers, the apps
 * and the dispatcher, after that it stops
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int HttpServer::start(int argc, char* argv[]) {
  bool stopApps = false;
  std::vector<HttpConnectionHandler *> connectionHandlers;
  try {
    if (this->analyzeArguments(argc, argv)) {
      // Start the log service
      Log::getInstance().start();

      // Start all web applications
      for (size_t index = 0; index < this->applications.size(); ++index) {
        this->applications[index]->start();
        this->applications[index]->startThread();
      }
      // start request dispatcher
      this->httpRequestDispatcher.startThread();
      stopApps = true;

      // Start waiting for connections
      this->listenForConnections(this->port);
      const NetworkAddress& address = this->getNetworkAddress();
      Log::append(Log::INFO, "webserver", "Listening on " + address.getIP()
        + " port " + std::to_string(address.getPort()));

      // initialize all connection handlers
      this->initConnectionHandlers(connectionHandlers, httpRequestDispatcher);

      // Accept all client connections
      this->acceptAllConnections();
    }
  } catch (const std::runtime_error& error) {
    // If applications were started
    if (stopApps) {
      // Stop web applications. Give them an opportunity to clean up
      for (size_t index = 0; index < this->applications.size(); ++index) {
        this->applications[index]->stop();
      }
    }
    // stop all connection handlers and their threads.
    this->stopHandlers(connectionHandlers);
    std::cerr << "error: " << error.what() << std::endl;
  }


  // Stop the log service
  Log::getInstance().stop();

  return EXIT_SUCCESS;
}

/**
 * @brief Method to initialize all connection handlers with their attributes
 * 
 * @param connectionHandlers 
 * @param httpRequestDispatcher 
 */

void HttpServer::initConnectionHandlers(std::vector<HttpConnectionHandler*>&
  connectionHandlers, HttpRequestDispatcher& httpRequestDispatcher) {
  // initialize pointers and initialize the queues to every
  // connection handler.
  for (size_t currentThread = 0;
    currentThread < this->connectionHandlerCount; ++currentThread) {
    connectionHandlers.push_back(new HttpConnectionHandler);
    connectionHandlers[currentThread]->
      setApplicationQueue(&this->applications);
    connectionHandlers[currentThread]->setConsumingQueue(this->clients);
    connectionHandlers[currentThread]->requestQueue =
      httpRequestDispatcher.getConsumingQueue();
  }

  // Starts all threads.
  for (size_t currentThread = 0;
    currentThread < this->connectionHandlerCount; ++currentThread) {
    connectionHandlers[currentThread]->startThread();
  }
}

/**
 * @brief Method to stop all connection handlers.
 * @details a dummy sockey is inserted in their consuming queue to signal
 * their stop. Also joins their threads
 * 
 * @param connectionHandlers 
 */

void HttpServer::stopHandlers(std::vector<HttpConnectionHandler*>&
  connectionHandlers) {
  // Stop flag for threads. avoid memory leaks
  for (size_t currentThread = 0;
    currentThread < this->connectionHandlerCount; ++currentThread) {
    connectionHandlers[currentThread]->getConsumingQueue()->push(Socket());
  }
  HttpReqRes request;
  this->httpRequestDispatcher.getConsumingQueue()->push(request);
  this->httpRequestDispatcher.waitToFinish();
  // Joining all threads to end it's execution.
  for (size_t currentThread = 0;
    currentThread < this->connectionHandlerCount; ++currentThread) {
    connectionHandlers[currentThread]->waitToFinish();
    delete connectionHandlers[currentThread];
  }
}

bool HttpServer::analyzeArguments(int argc, char* argv[]) {
  // Traverse all arguments
  for (int index = 1; index < argc; ++index) {
    const std::string argument = argv[index];
    if (argument.find("help") != std::string::npos) {
      std::cout << usage;
      return false;
    }
  }

  if (argc >= 2) {
    this->port = argv[1];
  }
  this->connectionHandlerCount = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc >= 3) {
    if (sscanf(argv[2], "%ld", &this->connectionHandlerCount) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return false;
    }
  }

  return true;
}

void HttpServer::stop() {
  // Stop listening for incoming client connection requests
  this->stopListening();
}
