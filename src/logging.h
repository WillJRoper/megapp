#ifndef LOGGING_H
#define LOGGING_H

// Includes
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include <stdexcept>
#include <string>

// Log levels
enum LogLevel { ERROR, LOG, VERBOSE };

/**
 * @brief The Logging class provides a simple mechanism for logging messages
 * to the standard output.
 *
 * The class is designed to provide a simple interface for logging messages
 * to the standard output. The implementation ensures proper formatting of
 * messages and allows for filtering based on a specified log level.
 *
 * Usage:
 * - Create an instance of the Logging class with the desired log level.
 * - Use the `message` function to log a message to the standard output.
 * - Use the `error` function to log an error message and throw an exception.
 * - Use the `tic` and `toc` functions to measure the duration of an operation.
 */
class Logging {
private:
  LogLevel _level; ///< The specified log level threshold.
  std::chrono::high_resolution_clock::time_point _tic; ///< Timer start point.
  std::chrono::high_resolution_clock::time_point _toc; ///< Timer end point.
  std::string error_message_; ///< The formatted error message.

public:
  /**
   * @brief Constructor for the Logging class.
   *
   * @param level The desired log level for filtering log messages.
   */
  Logging(LogLevel level) : _level(level) {}

  /**
   * @brief Log a verbose message.
   *
   * @tparam Args Variadic template for message formatting.
   *
   * @param format The format string for the log message.
   * @param args The arguments for message formatting.
   */
  template <typename... Args> void v_message(const char *format, Args... args) {
    if (_level >= 2) {
      log(format, args...);
    }
  }

  /**
   * @brief Log a regular log message.
   *
   * @tparam Args Variadic template for message formatting.
   *
   * @param format The format string for the log message.
   * @param args The arguments for message formatting.
   */
  template <typename... Args> void message(const char *format, Args... args) {
    if (_level >= LOG) {
      log(format, args...);
    }
  }

  /**
   * @brief Log an error message and throw a runtime error.
   *
   * @tparam Args Variadic template for message formatting.
   *
   * @param msg The error message format string.
   * @param args The arguments for message formatting.
   *
   * @throw std::runtime_error Thrown with the formatted error message.
   */
  template <typename... Args> void error(const std::string &msg, Args... args) {
    char buffer[256]; // Adjust the buffer size as needed.
    snprintf(buffer, sizeof(buffer), msg.c_str(), args...);
    std::ostringstream oss;
    oss << buffer;
    error_message_ = oss.str();
    throw std::runtime_error(error_message_);
  }

  /**
   * @brief Start measuring time.
   */
  void tic() { _tic = std::chrono::high_resolution_clock::now(); }

  /**
   * @brief Stop measuring time, log the duration, and print the log message.
   *
   * @param message The message indicating the operation being measured.
   */
  void toc(const char *message) {
    _toc = std::chrono::high_resolution_clock::now();

    /* Calculate the duration... */
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(_toc - _tic);

    log("%s took %lld microseconds", message, duration.count());
  }

private:
  /**
   * @brief Log a formatted message.
   *
   * @tparam Args Variadic template for message formatting.
   *
   * @param format The format string for the log message.
   * @param args The arguments for message formatting.
   */
  template <typename... Args> void log(const char *format, Args... args) {
    std::ostringstream oss;
    oss << " " << getStep() << "--> ";
    logMessage(oss, format, args...);
  }

  /**
   * @brief Get the current step of the simulation.
   *
   * @return The current step of the simulation.
   */
  std::string getStep() { return "[0000]"; }

  /**
   * @brief Log a formatted message.
   *
   * @tparam Args Variadic template for message formatting.
   *
   * @param oss The output string stream to write to.
   * @param format The format string for the log message.
   * @param args The arguments for message formatting.
   */
  template <typename... Args>
  void logMessage(std::ostringstream &oss, const char *format, Args... args) {
    char buffer[256]; // Adjust the buffer size as needed.
    snprintf(buffer, sizeof(buffer), format, args...);
    oss << buffer << std::endl;
    std::cout << oss.str();
  }
};

#endif // LOGGING_H
