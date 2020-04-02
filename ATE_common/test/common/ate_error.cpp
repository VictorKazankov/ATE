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

TEST(AteErrorTest, MakeErrorCode_InvalidDuration_Success) {
  auto code = common::make_error_code(common::AteError::kInvalidDuration);
  EXPECT_EQ(code, common::AteError::kInvalidDuration) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.message().c_str(), "Too long action duration");
}

TEST(AteErrorTest, MakeErrorCode_EmptyFileName_Success) {
  auto code = common::make_error_code(common::AteError::kEmptyFileName);
  EXPECT_EQ(code, common::AteError::kEmptyFileName) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.message().c_str(), "The screenshot's filename is empty");
}

TEST(AteErrorTest, MakeErrorCode_PermissionDenied_Success) {
  auto code = common::make_error_code(common::AteError::kPermissionDenied);
  EXPECT_EQ(code, common::AteError::kPermissionDenied) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.message().c_str(), "Permission denied");
}

TEST(AteErrorTest, MakeErrorCode_SystemError_Success) {
  auto code = common::make_error_code(common::AteError::kSystemError);
  EXPECT_EQ(code, common::AteError::kSystemError) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.message().c_str(), "System error");
}

TEST(AteErrorTest, MakeErrorCode_WrongExtension_Success) {
  auto code = common::make_error_code(common::AteError::kWrongExtension);
  EXPECT_EQ(code, common::AteError::kWrongExtension) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.message().c_str(), "Wrong extension");
}

TEST(AteErrorTest, MakeErrorCode_ImageAssemblingFailed_Success) {
  auto code = common::make_error_code(common::AteError::kImageAssemblingFailed);
  EXPECT_EQ(code, common::AteError::kImageAssemblingFailed) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.message().c_str(), "Image assembling failed");
}

TEST(AteErrorTest, MakeErrorrCode_WrongErrorType_Failure) {
  auto code = common::make_error_code(static_cast<common::AteError>(-1));
  EXPECT_NE(code, common::AteError::kPatternInvalid) << "Error code: " << code.message() << "\n";
  EXPECT_NE(code, common::AteError::kPatternNotFound) << "Error code: " << code.message() << "\n";
  EXPECT_NE(code, common::AteError::kVideoTemporarilyUnavailable) << "Error code: " << code.message() << "\n";
  EXPECT_NE(code, common::AteError::kInvalidDuration) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.message().c_str(), "Unrecognized Error");
}

TEST(AteErrorTest, MakeErrorCode_OutOfBoundaries_OutOfBoundaries) {
  auto code = common::make_error_code(common::AteError::kOutOfBoundaries);
  EXPECT_EQ(code, common::AteError::kOutOfBoundaries) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.message().c_str(), "Out of boundaries");
}

TEST(AteErrorTest, MakeErrorCode_WrongImageResolution_WrongImageResolution) {
  auto code = common::make_error_code(common::AteError::kWrongImageResolution);
  EXPECT_EQ(code, common::AteError::kWrongImageResolution) << "Error code: " << code.message() << "\n";
  EXPECT_STREQ(code.message().c_str(), "Wrong image resolution");
}

}  // namespace
