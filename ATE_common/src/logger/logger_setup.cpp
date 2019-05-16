#include "logger/logger_setup.h"

#include <memory>
#include <utility>
#include <vector>

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "common.h"
#include "exceptions.h"

namespace logger {

namespace {

using spdlog::level::level_enum;

struct Config {
  Config();

  bool console_enabled{false};
  spdlog::level::level_enum console_level{spdlog::level::off};
  bool file_enabled{false};
  spdlog::level::level_enum file_level{spdlog::level::off};
  std::size_t max_files{0};
  std::size_t max_file_size{0};
};

level_enum StrToLevel(const std::string& str) {
  level_enum level = spdlog::level::from_str(str);
  if (level < level_enum::trace || level > level_enum::critical) throw ConfigFormatError{};
  return level;
}

std::size_t IntToSize(const int size) {
  if (size < 0) throw ConfigFormatError{};
  return static_cast<std::size_t>(size);
}

Config::Config() {
  const std::string kLogSection = "LOG";
  const std::string kEnableConsoleSetting = "EnableConsoleLog";
  const std::string kConsoleLevelSetting = "ConsoleLogLevel";
  const std::string kEnableFileSetting = "EnableFileLog";
  const std::string kFileLevelSetting = "FileLogLevel";
  const std::string kMaxNumberOfLogFilesSetting = "MaxNumberOfFiles";
  const std::string kMaxSizeOfLogFileSetting = "MaxSizeOfSingleFile";

  console_enabled = common::Config().GetBool(kLogSection, kEnableConsoleSetting, false);
  console_level = StrToLevel(common::Config().GetString(kLogSection, kConsoleLevelSetting, "off"));

  file_enabled = common::Config().GetBool(kLogSection, kEnableFileSetting, false);
  file_level = StrToLevel(common::Config().GetString(kLogSection, kFileLevelSetting, "off"));

  max_files = IntToSize(common::Config().GetInt(kLogSection, kMaxNumberOfLogFilesSetting, -1));
  max_file_size = IntToSize(common::Config().GetInt(kLogSection, kMaxSizeOfLogFileSetting, -1));
}

}  // namespace

void SetUp() {
  constexpr auto kDefaultLoggerName = "ate_server_default_logger";
  constexpr auto kLoggerLevel = spdlog::level::trace;
  constexpr auto kLogFileName = "log.txt";
  constexpr auto kPattern = "[%t] [%Y-%m-%d %H:%M:%S:%F] [%^%l%$] %v";

  std::vector<spdlog::sink_ptr> sinks;
  Config config;

  if (config.console_enabled) {
    auto console_sink{std::make_shared<spdlog::sinks::stderr_color_sink_mt>()};
    console_sink->set_level(config.console_level);
    sinks.push_back(std::move(console_sink));
  }

  if (config.file_enabled) {
    constexpr std::size_t megabyte = 1024 * 1024;
    const std::size_t file_size_in_bytes = config.max_file_size * megabyte;
    auto file_sink{
        std::make_shared<spdlog::sinks::rotating_file_sink_mt>(kLogFileName, file_size_in_bytes, config.max_files)};
    file_sink->set_level(config.file_level);
    sinks.push_back(std::move(file_sink));
  }

  auto logger = std::make_shared<spdlog::logger>(kDefaultLoggerName, sinks.begin(), sinks.end());
  logger->set_level(kLoggerLevel);
  logger->set_pattern(kPattern);
  spdlog::set_default_logger(std::move(logger));
}

}  // namespace logger
