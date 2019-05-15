#include <cstddef>
#include <experimental/filesystem>
#include <string>

#include <gtest/gtest.h>

#include "common.h"
#include "config/config.h"
#include "exceptions.h"
#include "logger/logger.h"
#include "logger/logger_setup.h"

namespace {

namespace fs = std::experimental::filesystem;
namespace logger_to_test = logger::impl;

const std::string kConfigFilename = VHAT_COMMON_TEST_DATA_PATH "/logger/rotating_files_good_config.ini";
constexpr std::size_t kNumberOfFiles = 3;
constexpr std::size_t kKibi = 1024;
constexpr std::size_t kMebi = kKibi * kKibi;
constexpr std::size_t kMaxFileSize = kMebi;

class LoggerSetupTest : public ::testing::Test {
 protected:
  static void SetUpTestCase();
  static void TearDownTestCase();

  static const config::Reader config;
};

const config::Reader LoggerSetupTest::config{kConfigFilename};

void LoggerSetupTest::SetUpTestCase() {
  EXPECT_EQ(kMaxFileSize, kMebi * config.GetInt("LOG", "MaxSizeOfSingleFile", 0));
  EXPECT_EQ(kNumberOfFiles, config.GetInt("LOG", "MaxNumberOfFiles", 0));

  logger_to_test::SetUp(logger_to_test::Config{config});
}

void LoggerSetupTest::TearDownTestCase() {
  // Leave the global environment with default logger
  ASSERT_NO_THROW(common::SetUpLogger());
}

TEST_F(LoggerSetupTest, BadConstruction) {
  config::Reader config_invalid_size{VHAT_COMMON_TEST_DATA_PATH "/logger/invalid_size_bad_config.ini"};
  EXPECT_THROW(logger_to_test::SetUp(logger_to_test::Config{config_invalid_size}), logger::ConfigFormatError);

  config::Reader config_invalid_level{VHAT_COMMON_TEST_DATA_PATH "/logger/invalid_level_bad_config.ini"};
  EXPECT_THROW(logger_to_test::SetUp(logger_to_test::Config{config_invalid_level}), logger::ConfigFormatError);
}

TEST_F(LoggerSetupTest, DublicateInitialize) { EXPECT_NO_THROW(logger_to_test::SetUp(logger_to_test::Config{config})); }

void fill_log_files() {
  const std::string one_kilo_buffer(kKibi, 'a');
  for (std::size_t i = 0; i < kNumberOfFiles * kKibi; ++i) logger::critical(one_kilo_buffer);
}

fs::path get_log_name(std::size_t log_number) { return "log." + std::to_string(log_number) + ".txt"; }

TEST_F(LoggerSetupTest, FilesRotated) {
  fill_log_files();

  std::array<fs::path, kNumberOfFiles> generated_files;
  for (std::size_t i = 0; i < kNumberOfFiles; ++i) generated_files[i] = get_log_name(i + 1);

  for (const auto& file : generated_files) {
    std::error_code ec;
    EXPECT_TRUE(fs::exists(file, ec));
    EXPECT_EQ(ec.value(), 0);
  }

  const std::string non_existing_file = get_log_name(kNumberOfFiles + 1);
  std::error_code ec;
  EXPECT_FALSE(fs::exists(non_existing_file, ec));
  EXPECT_EQ(ec.value(), 0);
}

TEST_F(LoggerSetupTest, FileSizeCap) {
  ASSERT_GE(kNumberOfFiles, 1);
  fill_log_files();

  const std::string filled_to_cap_file = get_log_name(1);
  std::error_code ec;
  EXPECT_LE(fs::file_size(filled_to_cap_file, ec), kMaxFileSize);
  EXPECT_EQ(ec.value(), 0);
}

}  // namespace
