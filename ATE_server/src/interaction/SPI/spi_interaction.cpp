#include "interaction/SPI/spi_interaction.h"

#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "exceptions.h"
#include "logger/logger.h"

namespace {

const size_t kWidth = 1920;
const size_t kHeight = 1200;
const size_t kPackageSize = 8;
const uint32_t kDefaultSpeed = 1000000;

bool CheckResolution(size_t x, size_t y) { return !(x > kWidth || y > kHeight); }

}  // namespace

namespace interaction {

SpiInteraction::SpiInteraction(const std::string& device_address, defines::DisplayType /*display_type*/) {
  m_spi_ = open(device_address.c_str(), O_RDWR);

  if (m_spi_ < 0) {
    logger::error("[spiinteraction] Error open spi: {}", device_address);
    close(m_spi_);
    throw SpiOpenFailure{};
  }

  // configure bus
  if (need_bus_configuring_) {
    if (ioctl(m_spi_, SPI_IOC_WR_MAX_SPEED_HZ, &kDefaultSpeed) < 0) {
      logger::error("[spiinteraction] Error set speed");
      close(m_spi_);
      throw SpiOpenFailure{};
    }
  }
}

SpiInteraction::~SpiInteraction() {
  close(m_spi_);
  m_spi_ = -1;
}

void SpiInteraction::Tap(const int x, const int y) const {
  Press(x, y);
  Release(x, y);
}

void SpiInteraction::TouchAndDrag(const int x, const int y, const int dx, const int dy) const {
  Press(x, y);
  Drag(x + dx, y + dy);
  Release(x + dx, y + dy);
}

void SpiInteraction::Press(const int x, const int y) const { SendEvent(MouseEvent::MouseEvent_Press, x, y); }

void SpiInteraction::Release(const int x, const int y) const { SendEvent(MouseEvent::MouseEvent_Release, x, y); }

void SpiInteraction::Drag(const int x, const int y) const { SendEvent(MouseEvent::MouseEvent_Move, x, y); }

void SpiInteraction::SendEvent(SpiInteraction::MouseEvent evt, int x, int y) const {
  if (!CheckResolution(x, y)) {
    logger::error("[spiinteraction] Unavalible coordinates for resolution of screen: {}x{}", x, y);
    return;
  }

  std::vector<char> arr;

  arr.emplace_back(static_cast<char>(defines::ContactId::kOneFingerTouch));
  arr.emplace_back(static_cast<char>(0x00));
  arr.emplace_back(static_cast<char>(0x0));
  arr.emplace_back(static_cast<char>(evt & 0xFF));

  char xlsb = x & 0xff;
  char xmsb = (x >> 8) & 0xff;
  char ylsb = y & 0xff;
  char ymsb = (y >> 8) & 0xff;

  arr.emplace_back(xmsb);
  arr.emplace_back(xlsb);
  arr.emplace_back(ymsb);
  arr.emplace_back(ylsb);
  SendPacket(arr);
}

void SpiInteraction::SendPacket(std::vector<char>& data) const {
  const size_t size = data.size();

  if (size != kPackageSize) {
    for (size_t i = 0; i < kPackageSize - size; ++i) {
      data.emplace_back(static_cast<char>(0xFF));
    }
  }

  if (write(m_spi_, data.data(), kPackageSize) != kPackageSize) {
    logger::error("[spiinteraction] Spi write error");
    close(m_spi_);
    throw SpiWriteFailure{};
  }
}

}  // namespace interaction
