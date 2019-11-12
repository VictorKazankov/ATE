#include "utils/env_config.h"

#include <cstdlib>

#include "logger/logger.h"

namespace utils {

/**
 * @brief Get environment variable
 * @param key Environment variable name
 * @return Environment variable value
 */
std::string GetEnvVarValue(const std::string& key) {
  char const* value = std::getenv(key.c_str());
  return value == nullptr ? std::string() : std::string(value);
}

/**
 * @brief Get SPI device address written in environment variables
 * @return SPI device address
 */
std::string GetSpiConfig() {
  auto spi_bus = GetEnvVarValue("BOARD_SPI_BUS");
  auto spi_dev = GetEnvVarValue("BOARD_SPI_DEV");

  std::string device_address;
  if (!spi_bus.empty() && !spi_dev.empty()) {
    device_address += std::string("/dev/spidev") + spi_bus + "." + spi_dev;
    logger::info("[Parse Environment] SPI device address is: {}", device_address);
  } else {
    logger::critical("[Parse Environment] Can't read spi settings from environment variables");
  }

  return device_address;
}

std::string GetEnvSettings(const EnvSettings& setting) {
  switch (setting) {
    case EnvSettings::kSpiDeviceAddress: {
      return GetSpiConfig();
    }
  }
  return {};
}

}  // namespace utils
