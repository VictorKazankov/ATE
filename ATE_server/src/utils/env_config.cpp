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
    logger::info("[parse environment] SPI device address is: {}", device_address);
  } else {
    logger::critical("[parse environment] Can't read spi settings from environment variables");
  }

  return device_address;
}

std::string GetVideoStatusGpioPin() {
  auto gpio_enable = GetEnvVarValue("BOARD_GPIO_ENABLE");
  auto gpio_pin = GetEnvVarValue("BOARD_GPIO");

  if (!gpio_enable.empty() && !gpio_pin.empty()) {
    if (gpio_enable == "1") {
      logger::info("[parse environment] gpio pin of video status is {}", gpio_pin);
      return gpio_pin;
    } else {
      logger::warn("[parse environment] GPIO is disable, value from gpio is void");
    }
  } else {
    logger::error("[parse environment] Can't read gpio settings from environment variables");
  }

  return {};
}

/**
 * @brief Get GPIO file system path which derived by environment variables
 * @return GPIO file system path if configured and enable otherwise return empty string
 */
std::string GetVideoStatusGpioPath(const std::string& gpio_pin) {
  if (gpio_pin.empty()) {
    return {};
  }

  return std::string("/sys/class/gpio/gpio" + gpio_pin);
}

std::string GetEnvSettings(const EnvSettings& setting) {
  switch (setting) {
    case EnvSettings::kSpiDeviceAddress: {
      return GetSpiConfig();
    }
    case EnvSettings::kGpioVideoStatusPath: {
      return GetVideoStatusGpioPath(GetVideoStatusGpioPin());
    }
  }
  return {};
}

}  // namespace utils
