#include "interaction/SPI/spi_interaction.h"

#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cmath>

#include "exceptions.h"
#include "logger/logger.h"

namespace {

const size_t kPackageSize = 8;
const uint32_t kDefaultSpeed = 1000000;
const uint32_t kMinimalDragRange = 10;
constexpr auto kMillisecondsSendDelay = 25;

}  // namespace

namespace interaction {

SpiInteraction::SpiInteraction(const std::string& device_address, defines::DisplayType /*display_type*/,
                               int screen_width, int screen_height)
    : screen_width_(screen_width), screen_height_(screen_height) {
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
  int x2 = x + dx;
  int y2 = y + dy;

  auto distance = std::sqrt(dx * dx + dy * dy);
  int steps = static_cast<int>(std::ceil(distance) / kMinimalDragRange);

  // Coefficient of aspect ratio between points
  double k = 1.0;

  double x_tmp{};
  double y_tmp{};

  Press(x, y);

  for (int i = steps; i > 0; --i) {
    x_tmp = ((x + (k / i) * x2) / (1 + k / i));
    y_tmp = ((y + (k / i) * y2) / (1 + k / i));

    k += 1;
    Drag(static_cast<int>(x_tmp), static_cast<int>(y_tmp));
    std::this_thread::sleep_for(std::chrono::milliseconds(kMillisecondsSendDelay));
    logger::debug("[spiinteraction] Drag on x: {} y:{}", static_cast<int>(x_tmp), static_cast<int>(y_tmp));
  }

  Drag(x2, y2);
  Release(x2, y2);
}

void SpiInteraction::Press(const int x, const int y) const { SendEvent(MouseEvent::MouseEvent_Press, x, y); }

void SpiInteraction::Release(const int x, const int y) const { SendEvent(MouseEvent::MouseEvent_Release, x, y); }

void SpiInteraction::Drag(const int x, const int y) const { SendEvent(MouseEvent::MouseEvent_Move, x, y); }

void SpiInteraction::ChangeResolution(int screen_width, int screen_height) {
  screen_width_ = screen_width;
  screen_height_ = screen_height;
}

void SpiInteraction::SendEvent(SpiInteraction::MouseEvent evt, int x, int y) const {
  if (x > screen_width_ || y > screen_height_) {
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
