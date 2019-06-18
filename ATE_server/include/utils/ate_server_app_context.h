#ifndef ATE_COMMON_UTILS_ATE_SERVER_APP_CONTEXT_H_
#define ATE_COMMON_UTILS_ATE_SERVER_APP_CONTEXT_H_

#include <experimental/filesystem>
#include <string>

namespace fs = std::experimental::filesystem;

namespace utils {

/**
 * @brief Stores the main ATE server settings
 */
class ATEServerAppContext {
 public:
  ATEServerAppContext(int argc, const char* const argv[]);

  /**
   * @brief Indicates whether the server with the help option is run
   */
  bool HelpRequested() const noexcept;

  /**
   * @brief Help message
   */
  std::string HelpMessage() const;

  /**
   * @brief Path to the storage directory
   */
  const fs::path& StorageDir() const noexcept;

 private:
  bool help_requested_ = false;
  fs::path storage_dir_;
};
}  // namespace utils

#endif  // ATE_COMMON_UTILS_ATE_SERVER_APP_CONTEXT_H_
