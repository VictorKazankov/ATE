#include <gtest/gtest.h>

#include "utils/env_config.h"

namespace {

TEST(TestparseEnvironmentVariables, GetSpiDeviceName_ExistEnvVariable_Success) {
  auto spi_device_address = utils::GetEnvSettings(utils::EnvSettings::kSpiDeviceAddress);
  EXPECT_FALSE(spi_device_address.empty()) << "SPI device address must be not empty.\n";

  auto contain_spi_keyword = spi_device_address.find("/dev/spidev") != std::string::npos;
  EXPECT_TRUE(contain_spi_keyword) << "SPI device address must contain '/dev/spidev'.\n";
}

}  // namespace
