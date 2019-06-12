#include "utils/ate_server_app_context.h"

#include <opencv2/core/utility.hpp>

namespace {

constexpr auto kCmdLineArgs =
    "{help h usage ?||print this message}"
    "{config||config file path}"
    "{storage||storage path}";

}  // namespace

namespace utils {

ATEServerAppContext::ATEServerAppContext(int argc, const char* const argv[]) {
  cv::CommandLineParser cmd_parser{argc, argv, kCmdLineArgs};

  help_requested_ = cmd_parser.has("help");

  config_file_ = cmd_parser.get<std::string>("config");
  if (config_file_.empty()) {
    config_file_ = VHAT_SERVER_DATA_PATH "/vhat_server.ini";
  }

  storage_dir_ = cmd_parser.get<std::string>("storage");
  if (storage_dir_.empty()) {
    storage_dir_ = VHAT_SERVER_DATA_PATH "/storage";
  }
}

bool ATEServerAppContext::HelpRequested() const noexcept { return help_requested_; }

std::string ATEServerAppContext::HelpMessage() const {
  std::string help_message = "Supported command line arguments:\n{argument names|default value|help message}:\n";
  return help_message.append(kCmdLineArgs);
}

const fs::path& ATEServerAppContext::ConfigFile() const noexcept { return config_file_; }

const fs::path& ATEServerAppContext::StorageDir() const noexcept { return storage_dir_; }

}  // namespace utils
