#include <cstddef>
#include <experimental/filesystem>
#include <string>
#include <system_error>

#include <gtest/gtest.h>

#include "common.h"
#include "config/config.h"
#include "exceptions.h"
#include "logger/logger.h"
#include "logger/logger_setup.h"

namespace {

namespace fs = std::experimental::filesystem;

const std::string kConfigFilename = VHAT_COMMON_TEST_DATA_PATH "/logger/rotating_files_good_config.ini";
constexpr std::size_t kNumberOfFiles = 3;
constexpr std::size_t kKibi = 1024;
constexpr std::size_t kMebi = kKibi * kKibi;
constexpr std::size_t kMaxFileSize = kMebi;

class LoggerSetupTest : public ::testing::Test {
 protected:
  void SetUp() override;

  static void TearDownTestCase();
};

fs::path get_log_name(std::size_t log_number) { return "log." + std::to_string(log_number) + ".txt"; }

void clean_up_log_files() {
  for (std::size_t i = 0; i < kNumberOfFiles + 1; ++i) {
    std::error_code ec;
    fs::remove(get_log_name(i + 1), ec);  // do not expect neither true nor false, as file may or may not exist
    EXPECT_TRUE(!ec || ec == std::errc::no_such_file_or_directory) << ec.message() << " (" << ec << ')';
  }
}

void fill_log_files() {
  const std::string one_kilo_buffer(kKibi, 'a');
  for (std::size_t i = 0; i < kNumberOfFiles * kKibi; ++i) logger::critical(one_kilo_buffer);
}

void LoggerSetupTest::TearDownTestCase() {
  clean_up_log_files();

  // Leave the world with default logger
  const config::Reader default_config{VHAT_COMMON_TEST_DATA_PATH "/test_config.ini"};
  ASSERT_NO_THROW(logger::SetUp(default_config));
}

void LoggerSetupTest::SetUp() {
  const config::Reader config{kConfigFilename};

  EXPECT_EQ(kMaxFileSize, kMebi * config.GetInt("LOG", "MaxSizeOfSingleFile", 0));
  EXPECT_EQ(kNumberOfFiles, config.GetInt("LOG", "MaxNumberOfFiles", 0));

  EXPECT_NO_THROW(logger::SetUp(config));

  clean_up_log_files();
  fill_log_files();
}

TEST_F(LoggerSetupTest, BadConstruction) {
  const config::Reader config_invalid_size{VHAT_COMMON_TEST_DATA_PATH "/logger/invalid_size_bad_config.ini"};
  EXPECT_THROW(logger::SetUp(config_invalid_size), logger::ConfigFormatError);

  const config::Reader config_invalid_level{VHAT_COMMON_TEST_DATA_PATH "/logger/invalid_level_bad_config.ini"};
  EXPECT_THROW(logger::SetUp(config_invalid_level), logger::ConfigFormatError);
}

//TODO: FIXME Test is fail in Window. 
TEST_F(LoggerSetupTest, FilesRotated) {
  std::array<fs::path, kNumberOfFiles> generated_files;
  for (std::size_t i = 0; i < kNumberOfFiles; ++i) generated_files[i] = get_log_name(i + 1);

  for (const auto& file : generated_files) {
    std::error_code ec;
    EXPECT_TRUE(fs::exists(file, ec));
    EXPECT_FALSE(ec) << ec.message() << " (" << ec << ')';
  }

  const auto non_existing_file = get_log_name(kNumberOfFiles + 1);
  std::error_code ec;
  EXPECT_FALSE(fs::exists(non_existing_file, ec));
  EXPECT_TRUE(!ec || ec == std::errc::no_such_file_or_directory) << ec.message() << " (" << ec << ')';
}

//TODO: FIXME Test is fail in Window. 
TEST_F(LoggerSetupTest, FileSizeCap) {
  ASSERT_GE(kNumberOfFiles, 1);

  const auto filled_to_cap_file = get_log_name(1);
  std::error_code ec;
  EXPECT_LE(fs::file_size(filled_to_cap_file, ec), kMaxFileSize);
  EXPECT_FALSE(ec) << ec.message() << " (" << ec << ')';
}

}  // namespace
