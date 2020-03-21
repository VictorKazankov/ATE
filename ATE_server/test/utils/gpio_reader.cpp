#include <stdlib.h>
#include <experimental/filesystem>
#include <fstream>
#include <memory>
#include <string>

#include <gtest/gtest.h>

#include "utils/gpio_reader.h"
namespace {

namespace fs = std::experimental::filesystem;
using namespace utils;
using ::testing::Test;

class GpioReaderTest : public Test {
 protected:
  void SetUp() override;
  void TearDown() override;

  const fs::path gpio_path_ = "/tmp/gpio/gpio101";
  const std::string kValue = "value";
  std::unique_ptr<GpioReader> gpio_reader_;
};

void GpioReaderTest::SetUp() {
  std::error_code ec;
  fs::create_directories(gpio_path_, ec);
  EXPECT_EQ(ec, std::error_code{}) << "Gpio path directory should be crated but not error: " << ec.message();

  // Create attributes and their status.
  std::ofstream(gpio_path_ / kValue) << "0";

  // Create gpio reader
  gpio_reader_ = std::make_unique<GpioReader>(gpio_path_);
}

void GpioReaderTest::TearDown() {
  EXPECT_TRUE(fs::remove_all(gpio_path_)) << "Gpio path should be deleted after test";
  gpio_reader_.reset();
}

TEST_F(GpioReaderTest, CheckGpioAvailable_GpioNotSupported_Failure) {
  std::unique_ptr<GpioReader> reader_of_nothing = std::make_unique<GpioReader>("");
  EXPECT_FALSE(reader_of_nothing->CheckGpioAvailable()) << "Gpio should be unvailable";
}

TEST_F(GpioReaderTest, CheckGpioAvailable_GpioNotExist_Failure) {
  std::unique_ptr<GpioReader> reader_of_not_exist = std::make_unique<GpioReader>("/a/not/exit/path");
  EXPECT_FALSE(reader_of_not_exist->CheckGpioAvailable()) << "Gpio should be unavailable";
}

TEST_F(GpioReaderTest, CheckGpioAvailable_GpioUnknowValue_Failure) {
  std::ofstream(gpio_path_ / kValue) << "Invalid value";
  EXPECT_FALSE(gpio_reader_->CheckGpioAvailable()) << "Gpio should be unavailable";
}

TEST_F(GpioReaderTest, GetStatus_GpioUnvailable_Failure) {
  std::unique_ptr<GpioReader> reader_of_nothing = std::make_unique<GpioReader>("");
  EXPECT_FALSE(reader_of_nothing->GetStatus()) << "Status should be off";
}

TEST_F(GpioReaderTest, GetStatus_GpioOn_Sucess) {
  std::ofstream(gpio_path_ / kValue) << "1";
  EXPECT_TRUE(gpio_reader_->GetStatus()) << "Status should be on";
}

TEST_F(GpioReaderTest, GetStatus_GpioOff_Failure) {
  std::ofstream(gpio_path_ / kValue) << "0";
  EXPECT_FALSE(gpio_reader_->GetStatus()) << "Status should be off";
}

}  // namespace
