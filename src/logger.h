#pragma once

#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

#include "utils.h"

#include <sstream>

class Logger {

public:

  Logger(Logger const &) = delete;
  void operator=(const Logger &) = delete;

  inline static std::shared_ptr<spdlog::logger> &get_core_logger() {
    if (s_CoreLogger == nullptr)
      init();
    return s_CoreLogger;
  };
  inline static std::shared_ptr<spdlog::logger> &get_client_logger() {
    if (s_ClientLogger == nullptr)
      init();
    return s_ClientLogger;
  };

  inline static std::ostringstream &get_ostream() { return s_OStream; };

  template <typename... Args>
  static void log(Utils::LogLevel level, const Args &...msg) {

    switch (level) {
    case Utils::LogLevel::TRACE:
      s_CoreLogger->trace(msg...);
      break;

    case Utils::LogLevel::INFO:
      s_CoreLogger->info(msg...);
      break;

    case Utils::LogLevel::WARN:
      s_CoreLogger->warn(msg...);
      break;

    case Utils::LogLevel::ERROR:
      s_CoreLogger->error(msg...);
      break;
    }
    s_OStream.clear();
  }

private:
  static void init();

  static std::shared_ptr<spdlog::logger> s_CoreLogger;
  static std::shared_ptr<spdlog::logger> s_ClientLogger;
  static std::ostringstream s_OStream;
};

#define CORE_TRACE(...) ::Logger::get_core_logger()->trace(__VA_ARGS__);
#define CORE_INFO(...) ::Logger::get_core_logger()->info(__VA_ARGS__);
#define CORE_WARN(...) ::Logger::get_core_logger()->warn(__VA_ARGS__);
#define CORE_ERROR(...) ::Logger::get_core_logger()->error(__VA_ARGS__);

#define TRACE(...) ::Logger::get_client_logger()->trace(__VA_ARGS__);
#define INFO(...) ::Logger::get_client_logger()->info(__VA_ARGS__);
#define WARN(...) ::Logger::get_client_logger()->warn(__VA_ARGS__);
#define ERROR(...) ::Logger::get_client_logger()->error(__VA_ARGS__);
