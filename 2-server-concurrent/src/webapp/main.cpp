// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
// Serial web server's initial code for parallelization


#ifdef WEBSERVER
#include <signal.h>
#include <iostream>
#include "HttpServer.hpp"
#include "FactWebApp.hpp"

/// invoked when the process receives the signal (Ctrl+c or kill PID)
void signal_handler(int signal) {
  std::cout << "\nCaught signal " << signal << " coming out...\n";
  HttpServer::getInstance().stop();
}

/// Start the web server
int main(int argc, char* argv[]) {
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
  HttpServer &httpServer = HttpServer::getInstance();
  // Create a factorization web application, and other apps if you want
  FactWebApp factWebApp;
  // Register the web application(s) with the web server
  httpServer.chainWebApp(&factWebApp);
  // Start the web server
  return httpServer.start(argc, argv);
}

#endif  // WEBSERVER
