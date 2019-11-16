#include <gtest/gtest.h>

#include "utils/env_config.h"

namespace {

TEST(TestparseEnvironmentVariables, GetSpiDeviceName_ExistEnvVariable_Success) {
  auto spi_device_address = utils::GetEnvSettings(utils::EnvSettings::kSpiDeviceAddress);
  EXPECT_FALSE(spi_device_address.empty()) << "SPI device address must be not empty.\n";

  auto contain_spi_keyword = spi_device_address.find("/dev/spidev") != std::string::npos;
  EXPECT_TRUE(contain_spi_keyword) << "SPI device address must contain '/dev/spidev'.\n";
}

TEST(TestparseEnvironmentVariables, GetGpioVideoStatusPath_GpioEnable_Success) {
  // Setup the enviroment variable.
  setenv("BOARD_GPIO_ENABLE", "1", true);
  setenv("BOARD_GPIO", "10", true);

  auto gpio_path = utils::GetEnvSettings(utils::EnvSettings::kGpioVideoStatusPath);
  EXPECT_EQ(gpio_path, "/sys/class/gpio/gpio10");
}

TEST(TestparseEnvironmentVariables, GetGpioVideoStatusPath_GpioNotExist_EmptyPath) {
  // Setup the enviroment variable.
  unsetenv("BOARD_GPIO_ENABLE");
  unsetenv("BOARD_GPIO");

  auto gpio_path = utils::GetEnvSettings(utils::EnvSettings::kGpioVideoStatusPath);
  EXPECT_TRUE(gpio_path.empty()) << "gpio path must empty.\n";
}

TEST(TestparseEnvironmentVariables, GetGpioVideoStatusPath_GpioDisable_EmptyPath) {
  // Setup the enviroment variable.
  setenv("BOARD_GPIO_ENABLE", "0", true);

  auto gpio_path = utils::GetEnvSettings(utils::EnvSettings::kGpioVideoStatusPath);
  EXPECT_TRUE(gpio_path.empty()) << "gpio path must empty.\n";
}

}  // namespace
