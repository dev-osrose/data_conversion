#ifndef __CONSOLE_H_
#define __CONSOLE_H_

#include <ios>
#include <iomanip>
#include <iostream>
#include <spdlog/spdlog.h>

#ifdef _WIN32
  #include <spdlog/sinks/wincolor_sink.h>
#else
  #include <spdlog/sinks/ansicolor_sink.h>
#endif

enum class log_type {
  GENERAL,
  NETWORK,
  DATABASE,
  SYSTEM
};

class CLog {
public:
  static std::weak_ptr<spdlog::logger> GetLogger(log_type _type);
  static void SetLevel(spdlog::level::level_enum _level = spdlog::level::info);

private:
  static spdlog::level::level_enum level_;
};

#endif
