#include <gtest/gtest.h>

#include "exceptions.h"

namespace {

TEST(ExceptionsTest, What_FailedReadConfigFile_PredefinedMessage) {
  constexpr auto message_error1 = "Failed to read config file";
  static const std::string config_file = "file.ini";
  static const std::string message_error2 = "Failed to read config file: " + config_file;

  config::FileReadFailure file_read_failure1;
  config::FileReadFailure file_read_failure2(config_file);

  EXPECT_STREQ(message_error1, file_read_failure1.what());
  EXPECT_EQ(message_error2, file_read_failure2.what());
}

TEST(ExceptionsTest, What_InvalidLOGSectionConfigFile_PredefinedMessage) {
  constexpr auto message_error = "Invalid LOG section in config file";
  logger::ConfigFormatError config_format_error;
  EXPECT_STREQ(message_error, config_format_error.what());
}

TEST(ExceptionsTest, What_FailedConnectStorage_PredefinedMessage) {
  constexpr auto message_error = "Failed connect to storage";
  storage::ConnectionFailure connection_failure;
  EXPECT_STREQ(message_error, connection_failure.what());
}

TEST(ExceptionsTest, Code_BadFileDescriptor_PredefinedMessage) {
  interaction::SpiOpenFailure spi_open_failure;
  auto error_code = std::make_error_code(std::errc::bad_file_descriptor);
  EXPECT_EQ(error_code, spi_open_failure.code());
}

TEST(ExceptionsTest, Code_IOError_PredefinedMessage) {
  interaction::SpiWriteFailure spi_write_failure;
  auto error_code = std::make_error_code(std::errc::io_error);
  EXPECT_EQ(error_code, spi_write_failure.code());
}

TEST(ExceptionsTest, What_UndefinedTypeInteraction_PredefinedMessage) {
  constexpr auto message_error = "Undefined type of interaction";
  interaction::InteractionTypeError interaction_type_error;
  EXPECT_STREQ(message_error, interaction_type_error.what());
}

TEST(ExceptionsTest, What_InvalidVideoStreamConfig_PredefinedMessage) {
  constexpr auto message_error = "Invalid video stream config";
  streamer::InvalidConfig invalid_config;
  EXPECT_STREQ(message_error, invalid_config.what());
}

TEST(ExceptionsTest, What_MessageError_PredefinedMessage) {
  constexpr auto message_error = "message_error";
  streamer::StreamOpenFailure stream_open_failure(message_error);
  EXPECT_STREQ(message_error, stream_open_failure.what());
}

TEST(ExceptionsTest, What_UndefinedTypeImageMatching_PredefinedMessage) {
  constexpr auto message_error = "Undefined type of image matching";
  detector::ImageDetectorTypeError image_detector_type_error;
  EXPECT_STREQ(message_error, image_detector_type_error.what());
}

}  // namespace
