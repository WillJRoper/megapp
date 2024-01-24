#ifndef LOGGING_H
#define LOGGING_H

// Includes
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include <stdexcept>
#include <string>

enum LogLevel { ERROR, LOG, VERBOSE };

class Logging {
public:
  Logging(LogLevel level) : _level(level) {}

  // Variadic template function for logging a message with a specified level.
  template <typename... Args> void v_message(const char *format, Args... args) {
    if (_level >= 2) {
      log(format, args...);
    }
  }

  // Variadic template function for logging a log message with a specified
  // level.
  template <typename... Args> void message(const char *format, Args... args) {
    if (_level >= LOG) {
      log(format, args...);
    }
  }

  template <typename... Args> void error(const std::string &msg, Args... args) {
    char buffer[256]; // Adjust the buffer size as needed.
    snprintf(buffer, sizeof(buffer), msg.c_str(), args...);
    std::ostringstream oss;
    oss << buffer;
    error_message_ = oss.str();
    throw std::runtime_error(error_message_);
  }

  void tic() { _tic = std::chrono::high_resolution_clock::now(); }

  void toc(const char *message) {
    _toc = std::chrono::high_resolution_clock::now();

    /* Calculate the duration... */
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(_toc - _tic);

    log("%s took %lld microseconds", message, duration.count());
  }

private:
  LogLevel _level;
  std::chrono::high_resolution_clock::time_point _tic;
  std::chrono::high_resolution_clock::time_point _toc;
  std::string error_message_;

  // Variadic template function for formatting and logging.
  template <typename... Args> void log(const char *format, Args... args) {
    std::ostringstream oss;
    oss << " " << getStep() << "--> ";
    logMessage(oss, format, args...);
  }

  std::string getStep() {
    // Replace this with your MPI rank retrieval code.
    return "[0000]";
  }

  // Variadic template function for formatting the message.
  template <typename... Args>
  void logMessage(std::ostringstream &oss, const char *format, Args... args) {
    char buffer[256]; // Adjust the buffer size as needed.
    snprintf(buffer, sizeof(buffer), format, args...);
    oss << buffer << std::endl;
    std::cout << oss.str();
  }
};

#endif // LOGGING_H
