#include "utils/logger_setup.h"

#include <stdexcept>
#include <utility>
#include <vector>

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace logger {

namespace {

using spdlog::level::level_enum;

level_enum StrToLevel(const std::string& str) {
  level_enum level = spdlog::level::from_str(str);
  if (level < level_enum::trace || level > level_enum::critical)
    throw std::logic_error{"Invalid LOG section in config file"};
  return level;
}

std::size_t IntToSize(const int size) {
  if (size < 0) throw std::logic_error{"Invalid LOG section in config file"};
  return static_cast<std::size_t>(size);
}

}  // namespace

Setup::Config::Config(const config::Reader& config_reader) {
  const std::string kLogSection = "LOG";
  const std::string kEnableConsoleSetting = "EnableConsoleLog";
  const std::string kConsoleLevelSetting = "ConsoleLogLevel";
  const std::string kEnableFileSetting = "EnableFileLog";
  const std::string kFileLevelSetting = "FileLogLevel";
  const std::string kMaxNumberOfLogFilesSetting = "MaxNumberOfFiles";
  const std::string kMaxSizeOfLogFileSetting = "MaxSizeOfSingleFile";

  console_enabled = config_reader.GetBool(kLogSection, kEnableConsoleSetting, false);
  console_level = StrToLevel(config_reader.GetString(kLogSection, kConsoleLevelSetting, "off"));

  file_enabled = config_reader.GetBool(kLogSection, kEnableFileSetting, false);
  file_level = StrToLevel(config_reader.GetString(kLogSection, kFileLevelSetting, "off"));

  max_files = IntToSize(config_reader.GetInt(kLogSection, kMaxNumberOfLogFilesSetting, -1));
  max_file_size = IntToSize(config_reader.GetInt(kLogSection, kMaxSizeOfLogFileSetting, -1));
}

Setup::Setup(const config::Reader& config_reader) : config_{config_reader} {
  constexpr auto kDefaultLoggerName = "ate_server_default_logger";
  constexpr auto kLoggerLevel = spdlog::level::trace;
  constexpr auto kLogFileName = "log.txt";
  constexpr auto kPattern = "[%t] [%Y-%m-%d %H:%M:%S:%F] [%^%l%$] %v";

  std::vector<spdlog::sink_ptr> sinks;

  if (config_.console_enabled) {
    auto console_sink{std::make_shared<spdlog::sinks::stderr_color_sink_mt>()};
    console_sink->set_level(config_.console_level);
    sinks.push_back(std::move(console_sink));
  }

  if (config_.file_enabled) {
    constexpr std::size_t megabyte = 1024 * 1024;
    const std::size_t file_size_in_bytes = config_.max_file_size * megabyte;
    auto file_sink{
        std::make_shared<spdlog::sinks::rotating_file_sink_mt>(kLogFileName, file_size_in_bytes, config_.max_files)};
    file_sink->set_level(config_.file_level);
    sinks.push_back(std::move(file_sink));
  }

  auto logger = std::make_shared<spdlog::logger>(kDefaultLoggerName, sinks.begin(), sinks.end());
  logger->set_level(kLoggerLevel);
  logger->set_pattern(kPattern);
  spdlog::set_default_logger(std::move(logger));
}

}  // namespace logger
