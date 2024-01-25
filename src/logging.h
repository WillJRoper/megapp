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
 * Log levels:
 * - ERROR (0): Log only error messages. (Minimal output)
 * - LOG (1): Log regular messages. (Default)
 * - VERBOSE (2): Log verbose messages. (Maximum output)
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
  std::string error_message_;                          ///< The error message.
  char *error_file_;                                   ///< The error file.
  char *error_func_;                                   ///< The error function.
  int error_line_;                                     ///< The error line.

  // Singleton instance
  static Logging *instance;

  // Private constructor to prevent direct instantiation
  Logging(LogLevel level) : _level(level) {}

public:
  // Static method to get the singleton instance
  static Logging *getInstance(LogLevel level = LOG) {
    if (!instance) {
      instance = new Logging(level);
    }
    return instance;
  }

  // Static method to destroy the singleton instance
  static void destroyInstance() {
    if (instance) {
      delete instance;
      instance = nullptr;
    }
  }

  /**
   * @brief Log a verbose message.
   *
   * @tparam Args Variadic template for message formatting.
   *
   * @param format The format string for the log message.
   * @param args The arguments for message formatting.
   */
  template <typename... Args>
  void v_message(const char *file, const char *func, const char *format,
                 Args... args) {
    if (_level >= 2) {
      log(file, func, format, args...);
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
  template <typename... Args>
  void message(const char *file, const char *func, const char *format,
               Args... args) {
    if (_level >= LOG) {
      log(file, func, format, args...);
    }
  }

  template <typename... Args>
  void throw_error(const char *file, const char *func, const int line,
                   const char *format, Args... args) {
    // Hold on to information about the error
    char buffer[256];
    snprintf(buffer, sizeof(buffer), format, args...);
    this->error_message_ = buffer;
    this->error_file_ = const_cast<char *>(file);
    this->error_func_ = const_cast<char *>(func);
    this->error_line_ = line;

    // Throw the error
    throw std::runtime_error(std::string(buffer));
  }

  /**
   * @brief Log an error message and throw a runtime error.
   *
   * @param msg The error message format string.
   * @param args The arguments for message formatting.
   *
   * @throw std::runtime_error Thrown with the formatted error message.
   */
  void report_error() {
    std::ostringstream oss;
    oss << "[ERROR][" << getBaseFilename(this->error_file_) << "."
        << this->error_func_ << "." << this->error_line_
        << "]: " << this->error_message_;
    std::cerr << oss.str() << std::endl;
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
  void toc(const char *file, const char *func, const char *message) {
    _toc = std::chrono::high_resolution_clock::now();

    // Calculate the duration...
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(_toc - _tic);

    log(file, func, "%s took %lld microseconds", message, duration.count());
  }

private:
  /**
   * @brief Get the base filename from a given file path.
   *
   * @param filePath The full path to the file.
   * @return The base filename without the path and extension.
   */
  static std::string getBaseFilename(const std::string &filePath) {
    size_t lastSlash = filePath.find_last_of("/");
    size_t lastDot = filePath.find_last_of(".");

    // Extract the filename between the last slash and the last dot
    if (lastSlash != std::string::npos && lastDot != std::string::npos &&
        lastDot > lastSlash) {
      return filePath.substr(lastSlash + 1, lastDot - lastSlash - 1);
    }

    // If no slash or dot found, or dot appears before slash, return the
    // original path
    return filePath;
  }

  /**
   * @brief Log a formatted message.
   *
   * @tparam Args Variadic template for message formatting.
   *
   * @param format The format string for the log message.
   * @param args The arguments for message formatting.
   */
  template <typename... Args>
  void log(const char *file, const char *func, const char *format,
           Args... args) {
    std::ostringstream oss;
    oss << " " << getStep() << "[" << getBaseFilename(file) << "." << func
        << "]--> ";
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
    char buffer[256];
    snprintf(buffer, sizeof(buffer), format, args...);
    oss << buffer << std::endl;
    std::cout << oss.str();
  }
};

// Define friendly macros for logging
#define message(...)                                                           \
  Logging::getInstance()->message(__FILE__, __func__, __VA_ARGS__)
#define v_message(...)                                                         \
  Logging::getInstance()->v_message(__FILE__, __func__, __VA_ARGS__)
#define tic() Logging::getInstance()->tic()
#define toc(message) Logging::getInstance()->toc(__FILE__, __func__, message)
#define error(...)                                                             \
  Logging::getInstance()->throw_error(__FILE__, __func__, __LINE__, __VA_ARGS__)
#define report_error() Logging::getInstance()->report_error()

#endif // LOGGING_H
