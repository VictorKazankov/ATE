#include <fcntl.h>
#include <unistd.h>

#include <array>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

// Helper macros for testing private section
#define protected public
#define private public

#include "interaction/SPI/spi_interaction.h"

// TODO(SLisovenko@luxoft.com): technical debt - dummy for spi testing. Need refactoring in the future!
#define need_bus_configuring_ false
#include "../../../src/interaction/SPI/spi_interaction.cpp"

namespace {

const int kWidthResolution = 2348;
const int kHeightResolution = 1080;

class SpiInteractionTest : public ::testing::Test {
 protected:
  void SetUp() override;
  void TearDown() override;

  int pty_master_ = -1;  // pseudoterminal master
  int pty_slave_ = -1;   // pseudoterminal slave
  std::unique_ptr<interaction::SpiInteraction> spi_;
  std::array<unsigned char, 8> test_read_{};  // buffer for reading data from bus
};

void SpiInteractionTest::SetUp() {
  spi_ = std::make_unique<interaction::SpiInteraction>("/dev/null", kWidthResolution, kHeightResolution);
  EXPECT_FALSE(spi_ == nullptr);

  // open master pseudoterminal
  pty_master_ = open("/dev/ptmx", O_RDWR);
  ASSERT_LE(0, pty_master_);
  auto rc = grantpt(pty_master_);
  EXPECT_EQ(0, rc);
  rc = unlockpt(pty_master_);
  EXPECT_EQ(0, rc);

  // open slave pseudoterminal in nonblock mode
  pty_slave_ = open(ptsname(pty_master_), O_RDWR | O_NONBLOCK);
  ASSERT_LE(0, pty_slave_);

  // change descriptor for spi to pty slave
  spi_->m_spi_ = pty_slave_;
}

void SpiInteractionTest::TearDown() {
  close(pty_master_);
  close(pty_slave_);
}

TEST_F(SpiInteractionTest, SpiInteraction_CheckSpecifiedResolution_Equal) {
  EXPECT_EQ(spi_->screen_width_, kWidthResolution);
  EXPECT_EQ(spi_->screen_height_, kHeightResolution);
}

TEST_F(SpiInteractionTest, SendPacketSuccess) {
  std::vector<char> test_write{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

  // write spi
  spi_->SendPacket(test_write);
  auto readed = read(pty_master_, test_read_.data(), test_read_.size());

  EXPECT_EQ(readed, test_write.size());
  EXPECT_EQ(0, memcmp(test_write.data(), test_read_.data(), test_write.size()));
}

TEST_F(SpiInteractionTest, SendPacketEmptyDataSuccess) {
  std::vector<char> test_write{};
  std::array<unsigned char, 8> etalon{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

  // write spi
  spi_->SendPacket(test_write);
  auto readed = read(pty_master_, test_read_.data(), test_read_.size());

  EXPECT_EQ(readed, test_write.size());
  EXPECT_TRUE(test_read_ == etalon);
}

TEST_F(SpiInteractionTest, PressSuccess) {
  std::array<unsigned char, 8> etalon{0x00, 0x00, 0x00, 'P', 0x00, 0x64, 0x00, 0x64};

  // write spi
  spi_->Press(100, 100);
  [[gnu::unused]] auto readed = read(pty_master_, test_read_.data(), test_read_.size());

  EXPECT_TRUE(test_read_ == etalon);
}

TEST_F(SpiInteractionTest, ReleaseSuccess) {
  std::array<unsigned char, 8> etalon{0x00, 0x00, 0x00, 'R', 0x00, 0x64, 0x00, 0x64};

  // write spi
  spi_->Release(100, 100);
  [[gnu::unused]] auto readed = read(pty_master_, test_read_.data(), test_read_.size());

  EXPECT_TRUE(test_read_ == etalon);
}

TEST_F(SpiInteractionTest, DragSuccess) {
  std::array<unsigned char, 8> etalon{0x00, 0x00, 0x00, 'M', 0x00, 0x64, 0x00, 0x64};

  // write spi
  spi_->Drag(100, 100);
  [[gnu::unused]] auto readed = read(pty_master_, test_read_.data(), test_read_.size());

  EXPECT_TRUE(test_read_ == etalon);
}

TEST_F(SpiInteractionTest, TapSuccess) {
  std::array<unsigned char, 8> etalon{0x00, 0x00, 0x00, 'P', 0x00, 0x64, 0x00, 0x64};

  // write spi
  spi_->Tap(100, 100);
  [[gnu::unused]] auto readed = read(pty_master_, test_read_.data(), test_read_.size());

  EXPECT_TRUE(test_read_ == etalon);
}

TEST_F(SpiInteractionTest, ChangeResolution_CheckOriginalWidthHeight_Correct) {
  const int kNewWidth = 800;
  const int kNewHeight = 600;
  spi_->ChangeResolution(kNewWidth, kNewHeight);

  EXPECT_EQ(spi_->screen_width_, kNewWidth);
  EXPECT_EQ(spi_->screen_height_, kNewHeight);
}

}  // namespace
