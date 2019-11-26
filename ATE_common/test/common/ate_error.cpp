#include <gtest/gtest.h>

#include "ate_error.h"

namespace {

TEST(AteErrorTest, MakeErrorCode_PatternInvalid_Success) {
  auto code = common::make_error_code(common::AteError::kPatternInvalid);
  EXPECT_EQ(code, common::AteError::kPatternInvalid) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.category().name(), "ATE");
  EXPECT_STREQ(code.message().c_str(), "Pattern is invalid");
}

TEST(AteErrorTest, MakeErrorCode_PatternNotFound_Success) {
  auto code = common::make_error_code(common::AteError::kPatternNotFound);
  EXPECT_EQ(code, common::AteError::kPatternNotFound) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.message().c_str(), "Pattern is not found");
}

TEST(AteErrorTest, MakeErrorCode_VideoTemporarilyUnavailable_Success) {
  auto code = common::make_error_code(common::AteError::kVideoTemporarilyUnavailable);
  EXPECT_EQ(code, common::AteError::kVideoTemporarilyUnavailable) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.message().c_str(), "Video output is temporary unavailable");
}

TEST(AteErrorTest, MakeErrorCode_InvalidDurationLongPress_Success) {
  auto code = common::make_error_code(common::AteError::kInvalidDurationLongPress);
  EXPECT_EQ(code, common::AteError::kInvalidDurationLongPress) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.message().c_str(), "Too long press action duration");
}

TEST(AteErrorTest, MakeErrorrCode_WrongErrorType_Failure) {
  auto code = common::make_error_code(static_cast<common::AteError>(-1));
  EXPECT_NE(code, common::AteError::kPatternInvalid) << "Error code: " << code.message() << "\n";
  EXPECT_NE(code, common::AteError::kPatternNotFound) << "Error code: " << code.message() << "\n";
  EXPECT_NE(code, common::AteError::kVideoTemporarilyUnavailable) << "Error code: " << code.message() << "\n";
  EXPECT_NE(code, common::AteError::kInvalidDurationLongPress) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.message().c_str(), "Unrecognized Error");
}

}  // namespace
