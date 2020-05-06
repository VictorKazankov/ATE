#include <gtest/gtest.h>

#include "error_defines.h"

namespace interaction {

TEST(ErrorDefinesTest, What_VideoStreamNotFound_ExpectedErrorMessage) {
  constexpr auto message_error = "Video stream not found";
  VideoStreamingError video_stream_error;
  EXPECT_STREQ(message_error, video_stream_error.what());
}

TEST(ErrorDefinesTest, What_NoConnectionEstablished_ExpectedErrorMessage) {
  constexpr auto message_error = "No connection to server-side exists."
                                 " First you need to perform attachToApplication API";
  NoConnectionEstablished no_connection_established_error;
  EXPECT_STREQ(message_error, no_connection_established_error.what());
}

TEST(ErrorDefinesTest, What_NoAvailableDiskSpace_ExpectedErrorMessage) {
  constexpr auto message_error = "No free space on a disk";
  NoAvailableDiskSpace no_available_disk_space_error;
  EXPECT_STREQ(message_error, no_available_disk_space_error.what());
}

}  // namespace interaction
