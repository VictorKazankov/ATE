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

TEST_F(GpioReaderTest, GetStatus_GpioNotSupported_Success) {
  std::unique_ptr<GpioReader> reader = std::make_unique<GpioReader>("");
  EXPECT_TRUE(reader->GetStatus()) << "Status should be on";
}

TEST_F(GpioReaderTest, GetStatus_GpioOn_Sucess) {
  std::ofstream(gpio_path_ / kValue) << "1";
  EXPECT_TRUE(gpio_reader_->GetStatus()) << "Status should be on";
}

TEST_F(GpioReaderTest, GetStatus_GpioOff_Sucess) {
  std::ofstream(gpio_path_ / kValue) << "0";
  EXPECT_FALSE(gpio_reader_->GetStatus()) << "Status should be off";
}

TEST_F(GpioReaderTest, GetStatus_GpioUnknowValue_Failure) {
  std::ofstream(gpio_path_ / "value") << "unknown value";
  EXPECT_FALSE(gpio_reader_->GetStatus()) << "Gpio should not return the status";
}

TEST_F(GpioReaderTest, GetStatus_GpioNotExist_Failure) {
  GpioReader gpio_reader_not_exit("/a/not/exit/path");
  EXPECT_FALSE(gpio_reader_not_exit.GetStatus()) << "Gpio should not return the status";
}

}  // namespace
