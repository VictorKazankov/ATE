#include <gtest/gtest.h>

#include "error_defines.h"
#include "json_rpc_parser.h"
#include "message_factory/json_defines.h"
#include "message_factory/json_utils.h"

namespace {

static const std::string valid_json_structure{R"({"id":10,"jsonrpc":"2.0","result":true})"};
static const std::string invalid_json_structure{R"({"wrong-id":1,"wrong-jsonrpc":"2.0","wrong-result":true})"};

TEST(ExtractGetImagesDiscrepancyResponseParamsTest,
     ExtractGetImagesDiscrepancyResponseParams_ValidRequest_ExpectedParams) {
  int expected_discrepancy = 10;
  char request[256]{};
  snprintf(request, sizeof(request) / sizeof(char), R"({"id":1,"jsonrpc":"2.0","result":{"discrepancy_percent":%d}})",
           expected_discrepancy);
  auto discrepancy = interaction::JsonRpcParser::ParseGetImagesDiscrepancy(request);
  EXPECT_EQ(discrepancy, expected_discrepancy) << "The result does not match the expected value.";
}

TEST(ExtractGetImagesDiscrepancyResponseParamsTest,
     ExtractGetImagesDiscrepancyResponseParams_InvalidRequest_RuntimeError) {
  const auto request = R"({"id":1,"jsonrpc":"2.0","result":{"invalid_name":10}})";
  EXPECT_THROW(interaction::JsonRpcParser::ParseGetImagesDiscrepancy(request), std::runtime_error);
}

TEST(JsonRpcParserTest, IsJsonStructureValid_ValidJsonStructure_ExpectTrue) {
  Json::Value schema;
  int id = 10;
  bool result = true;

  EXPECT_TRUE(interaction::impl::IsJsonStructureValid(valid_json_structure, schema));
  EXPECT_EQ(schema[common::jmsg::kId].asInt(), id);
  EXPECT_STREQ(schema[common::jmsg::kJsonRpc].asCString(), common::jmsg::kRpcVersion);
  EXPECT_EQ(schema[common::jmsg::kResult].asBool(), result);
}

TEST(JsonRpcParserTest, IsJsonStructureValid_InvalidJsonStructure_ExpectFalse) {
  Json::Value schema;

  EXPECT_FALSE(interaction::impl::IsJsonStructureValid(invalid_json_structure, schema));
}

TEST(JsonRpcParserTest, CheckAndRaiseExceptionInCaseErrors_ValidJsonStructure_ExpectNoThrow) {
  EXPECT_NO_THROW(interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(valid_json_structure));
}

TEST(JsonRpcParserTest, CheckAndRaiseExceptionInCaseErrors_JsonStructureWithError_RuntimeError) {
  static const std::string json_structure{R"({"id":1,"jsonrpc":"2.0","error":{"code":-32700}})"};
  EXPECT_THROW(interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(json_structure), std::runtime_error);
}

TEST(JsonRpcParserTest, ErrorHandler_InvalidParams_InvalidArgumentError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kInvalidParams);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), std::invalid_argument);
}

TEST(JsonRpcParserTest, ErrorHandler_ObjectNotFound_LookupError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kObjectNotFound);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), squish::LookupError);
}

TEST(JsonRpcParserTest, ErrorHandler_VideoStreamNotFound_VideoStreamingError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kVideoStreamNotFound);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), interaction::VideoStreamingError);
}

TEST(JsonRpcParserTest, ErrorHandler_InternalError_RuntimeError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kInternalError);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), std::runtime_error);
}

TEST(JsonRpcParserTest, ErrorHandler_ParseError_RuntimeError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kParseError);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), std::runtime_error);
}

TEST(JsonRpcParserTest, ErrorHandler_InvalidRequest_RuntimeError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kInvalidRequest);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), std::runtime_error);
}

TEST(JsonRpcParserTest, ErrorHandler_MethodNotFound_RuntimeError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kMethodNotFound);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), std::runtime_error);
}

TEST(JsonRpcParserTest, ErrorHandler_InvalidSyncVersion_InvalidSyncVersionError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kInvalidSyncVersion);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), squish::InvalidSyncVersion);
}

TEST(JsonRpcParserTest, ErrorHandler_InvalidSyncBuildVersion_InvalidSyncBuildVersionError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kInvalidSyncBuildVersion);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), squish::InvalidSyncBuildVersion);
}

TEST(JsonRpcParserTest, ErrorHandler_InvalidSyncCollectionMode_InvalidSyncCollectionModeError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kInvalidSyncCollectionMode);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), squish::InvalidSyncCollectionMode);
}

TEST(JsonRpcParserTest, ErrorHandler_InvalidDuration_InvalidDurationError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kInvalidDuration);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), squish::InvalidDuration);
}

TEST(JsonRpcParserTest, ErrorHandler_EmptyFileName_EmptyScreenshotFileNameError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kEmptyFileName);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), squish::EmptyScreenshotFileName);
}

TEST(JsonRpcParserTest, ErrorHandler_WrongExtension_WrongScreenshotExtensionError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kWrongExtension);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), squish::WrongScreenshotExtension);
}

TEST(JsonRpcParserTest, ErrorHandler_PermissionDenied_PermissionDeniedError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kPermissionDenied);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), squish::PermissionDenied);
}

TEST(JsonRpcParserTest, ErrorHandler_ImageAssemblingFailed_ImageAssemblingFailedError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kImageAssemblingFailed);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), squish::ImageAssemblingFailed);
}

TEST(JsonRpcParserTest, ErrorHandler_InvalidRectangleCoordinates_InvalidRectangleCoordinatesError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kInvalidRectangleCoordinates);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), squish::InvalidRectangleCoordinates);
}

TEST(JsonRpcParserTest, ErrorHandler_ComparingImageIncorrectSize_ComparingImageIncorrectSizeError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kComparingImageIncorrectSize);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), squish::ComparingImageIncorrectSize);
}

TEST(JsonRpcParserTest, ErrorHandler_ComparingImageNotExist_ComparingImageNotExistError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kComparingImageNotExist);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), squish::ComparingImageNotExist);
}

TEST(JsonRpcParserTest, ErrorHandler_UnsupportedFileType_UnsupportedFileTypeError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kUnsupportedFileType);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), squish::UnsupportedFileType);
}

TEST(JsonRpcParserTest, ErrorHandler_NoAvailableDiskSpace_NoAvailableDiskSpaceError) {
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = static_cast<int>(rpc::Error::kNoAvailableDiskSpace);

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), interaction::NoAvailableDiskSpace);
}

TEST(JsonRpcParserTest, ErrorHandler_UndefinedError_RuntimeError) {
  const int unknown_error = -1;
  Json::Value schema;
  schema[common::jmsg::kError][common::jmsg::kErrorCode] = unknown_error;

  EXPECT_THROW(interaction::impl::ErrorHandler(schema[common::jmsg::kError]), std::runtime_error);
}

TEST(JsonRpcParserTest, ParseGetScreenshot_ResponseWithTrueResult_Success) {
  const auto& response = valid_json_structure;
  EXPECT_TRUE(interaction::JsonRpcParser::ParseGetScreenshot(response));
}

TEST(JsonRpcParserTest, ParseGetScreenshot_ResponseWithFalseResult_Failed) {
  const std::string response{R"({"id":10,"jsonrpc":"2.0","result":false})"};
  EXPECT_FALSE(interaction::JsonRpcParser::ParseGetScreenshot(response));
}

TEST(JsonRpcParserTest, ParseGetText_ResponseWithTextResult_Success) {
  const std::string response{R"({"id":10,"jsonrpc":"2.0","result":{"text":"test"}})"};
  EXPECT_EQ("test", interaction::JsonRpcParser::ParseGetText(response));
}

TEST(JsonRpcParserTest, ParseGetText_InvalidResponse_EmptyString) {
  EXPECT_EQ(std::string{}, interaction::JsonRpcParser::ParseGetText(invalid_json_structure));
}

TEST(JsonRpcParserTest, ParseGetText_ResponseWithInvalidKey_EmptyString) {
  const std::string response{R"({"id":10,"jsonrpc":"2.0","result":{"invalid_key":"test"}})"};
  EXPECT_EQ(std::string{}, interaction::JsonRpcParser::ParseGetText(response));
}

TEST(JsonRpcParserTest, ParseGetText_ResponseWithError_RuntimeError) {
  const std::string response{R"({"id":10,"jsonrpc":"2.0","error":{"code":-1}})"};
  EXPECT_THROW(interaction::JsonRpcParser::ParseGetText(response), std::runtime_error);
}

TEST(JsonRpcParserTest, ParseGetObjectsDataByPattern_ResponseWithInvalidParam_ObjectListEmpty) {
  const std::string response{
      R"({"id":10,"jsonrpc":"2.0","result":{"x":"invalid_param", "y":10,"width":20,"height":30}})"};
  auto objects_list = interaction::JsonRpcParser::ParseGetObjectsDataByPattern(response);
  EXPECT_TRUE(objects_list.empty());
}

TEST(JsonRpcParserTest, ParseCaptureFrames_ResponseWithInvalidParam_RuntimeError) {
  const std::string response{
      R"({"id":10,"jsonrpc":"2.0","result":{"filename":["frame1.png","frame2.png","frame3.png"]},
       "error":{"code":-32700}})"};
  EXPECT_THROW(interaction::JsonRpcParser::ParseCaptureFrames(response), std::runtime_error);
}

TEST(JsonRpcParserTest, ParseCaptureFrames_ResponseWithValidParams_ValidFileList) {
  const std::string response{
      R"({"id":10,"jsonrpc":"2.0","result":{"filename":["frame1.png","frame2.png","frame3.png"]}})"};
  std::vector<std::string> expected_result{"frame1.png", "frame2.png", "frame3.png"};

  auto file_list = interaction::JsonRpcParser::ParseCaptureFrames(response);

  EXPECT_EQ(expected_result, file_list);
}

}  // namespace
