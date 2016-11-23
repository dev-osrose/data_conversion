#include <sstream>
#include "logconsole.h"

spdlog::level::level_enum CLog::level_ = spdlog::level::info;

void CLog::SetLevel(spdlog::level::level_enum _level) {
  level_ = _level;

  std::ostringstream format;
  format << "[%H:%M:%S.%e %z] [%L]";

  if (level_ <= spdlog::level::debug) format << " [thread %t]";
  format << " [%n]" << " %v ";
  spdlog::set_pattern(format.str());
}

std::weak_ptr<spdlog::logger> CLog::GetLogger(
    log_type _type) {
  std::weak_ptr<spdlog::logger> logger;
  try
  {
    switch (_type) {
      case log_type::GENERAL:
      default:
        logger = spdlog::get("data_conv");
        break;
    }

    if (logger.expired()) {

      std::vector<spdlog::sink_ptr> net_sink;
#ifdef _WIN32
      auto console_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
      net_sink.push_back(console_sink);
#else
      auto console_sink = std::make_shared<spdlog::sinks::ansicolor_sink>(spdlog::sinks::stdout_sink_mt::instance());
      
      #ifdef SPDLOG_ENABLE_SYSLOG
        auto syslog_sink = std::make_shared<spdlog::sinks::syslog_sink>("data_conv");
        net_sink.push_back(syslog_sink);
      #endif
      net_sink.push_back(console_sink);
#endif

      auto net_logger = std::make_shared<spdlog::logger>(
          "data_conv", begin(net_sink), end(net_sink));
      net_logger->set_level(level_);
      spdlog::register_logger(net_logger);

      return net_logger;
    }
  } catch (const spdlog::spdlog_ex& ex) {
    std::cout << "Log failed: " << ex.what() << std::endl;
  }
  return logger;
}
