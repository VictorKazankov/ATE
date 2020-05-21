#include <gtest/gtest.h>

#include "message_factory/json_defines.h"
#include "message_factory/json_messages.h"
#include "message_factory/json_utils.h"
#include "message_factory/message_factory.h"
#include "rpc_error.h"
#include "utils/squish_types_converter.h"

namespace {

constexpr auto correct_json_string = R"({"id":1,"jsonrpc":"2.0","result":true})";

void WrongRequestTest(const char* message, rpc::Error expected_error, std::uint64_t expected_id = 0) {
  std::uint64_t id;
  std::string method;
  Json::Value params;
  Json::Value error;

  common::jmsg::ParseJsonRpcRequest(message, id, method, params, error);

  EXPECT_EQ(id, expected_id) << "Id must be equal " << expected_id;
  EXPECT_FALSE(error.empty()) << "Error must be non-empty";
  int ec = 0;
  try {
    ec = error[common::jmsg::kErrorCode].asInt();
  } catch (const Json::LogicError& invalid_error_object) {
    EXPECT_TRUE(false) << "Error code must be convertable to int: " << invalid_error_object.what();
  }
  EXPECT_EQ(ec, static_cast<int>(expected_error)) << "Wrong error code";
}

void RightRequestTest(const char* message) {
  std::uint64_t id;
  std::string method;
  Json::Value params;
  Json::Value error;

  common::jmsg::ParseJsonRpcRequest(message, id, method, params, error);

  EXPECT_TRUE(id) << "Id must be non-zero";
  EXPECT_TRUE(error.empty()) << "Error must be empty";
}

void WrongWaitForObjectParamsTest(const char* message) {
  common::ObjectDataIdentity object_data_identity;
  std::chrono::milliseconds timeout;
  common::Point top_left;
  common::Point bottom_right;
  Json::Value error;

  common::jmsg::ExtractWaitForObjectRequestParams(message, object_data_identity, timeout, top_left, bottom_right,
                                                  error);

  EXPECT_FALSE(error.empty()) << "Error must be non-empty";
  int ec = 0;
  try {
    ec = error[common::jmsg::kErrorCode].asInt();
  } catch (const Json::LogicError& invalid_error_object) {
    EXPECT_TRUE(false) << "Error code must be convertable to int: " << invalid_error_object.what();
  }
  EXPECT_EQ(ec, static_cast<int>(rpc::Error::kInvalidParams)) << "Wrong error code";
}

void WrongTapObjectParamsTest(const char* message) {
  int x;
  int y;
  common::squish::ModifierState modifier_state = common::squish::ModifierState::kNone;
  common::squish::MouseButton mouse_button = common::squish::MouseButton::kNone;
  Json::Value error;

  common::jmsg::ExtractTapObjectRequestParams(message, x, y, modifier_state, mouse_button, error);

  EXPECT_FALSE(error.empty()) << "Error must be non-empty";
  int ec = 0;
  try {
    ec = error[common::jmsg::kErrorCode].asInt();
  } catch (const Json::LogicError& invalid_error_object) {
    EXPECT_TRUE(false) << "Error code must be convertable to int: " << invalid_error_object.what();
  }
  EXPECT_EQ(ec, static_cast<int>(rpc::Error::kInvalidParams)) << "Wrong error code";
}

TEST(RpcRequestTest, InvalidJson) { WrongRequestTest("There is no valid jsonrpc message", rpc::Error::kParseError); }

TEST(RpcRequestTest, WrongId) {
  // Negative int
  WrongRequestTest("{\"jsonrpc\":\"2.0\",\"id\":-1,\"method\":\"m\"}", rpc::Error::kInvalidRequest);

  // Zero int
  WrongRequestTest("{\"jsonrpc\":\"2.0\",\"id\":0,\"method\":\"m\"}", rpc::Error::kInvalidRequest);

  // TODO: FIXME Test is fail in Window. Too big is not Too big for Window
  // Too big int
  WrongRequestTest("{\"jsonrpc\":\"2.0\",\"id\":18446744073709551616,\"method\":\"m\"}", rpc::Error::kInvalidRequest);

  // String
  WrongRequestTest("{\"jsonrpc\":\"2.0\",\"id\":\"1\",\"method\":\"m\"}", rpc::Error::kInvalidRequest);

  // Float
  WrongRequestTest("{\"jsonrpc\":\"2.0\",\"id\":1.9,\"method\":\"m\"}", rpc::Error::kInvalidRequest);

  // Null
  WrongRequestTest("{\"jsonrpc\":\"2.0\",\"id\":null,\"method\":\"m\"}", rpc::Error::kInvalidRequest);
}

TEST(RpcRequestTest, WrongProtocolVersion) {
  // Int
  WrongRequestTest("{\"jsonrpc\":2,\"id\":1,\"method\":\"m\"}", rpc::Error::kInvalidRequest, 1);

  // Float
  WrongRequestTest("{\"jsonrpc\":2.0,\"id\":1,\"method\":\"m\"}", rpc::Error::kInvalidRequest, 1);

  // Wrong version
  WrongRequestTest("{\"jsonrpc\":\"1.0\",\"id\":1,\"method\":\"m\"}", rpc::Error::kInvalidRequest, 1);
}

TEST(RpcRequestTest, WrongMethod) {
  // Int
  WrongRequestTest("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":2}", rpc::Error::kInvalidRequest, 1);

  // Float
  WrongRequestTest("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":3.9}", rpc::Error::kInvalidRequest, 1);

  // Null
  WrongRequestTest("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":null}", rpc::Error::kInvalidRequest, 1);

  // Bool
  WrongRequestTest("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":true}", rpc::Error::kInvalidRequest, 1);
}

TEST(RpcRequestTest, RightId) {
  RightRequestTest("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"m\"}");

  // Max uint64
  RightRequestTest("{\"jsonrpc\":\"2.0\",\"id\":18446744073709551615,\"method\":\"m\"}");
}

TEST(RpcRequestTest, WrongWaitForObjectRequest) {
  // Negative timeout
  WrongWaitForObjectParamsTest("{\"object_or_name\":\"o\",\"timeout_msec\":-1}");

  // Too big timeout
  WrongWaitForObjectParamsTest("{\"object_or_name\":\"o\",\"timeout_msec\":4294967296}");

  // Without object_or_name
  WrongWaitForObjectParamsTest("{\"timeout_msec\":1}");

  // Without timeout
  WrongWaitForObjectParamsTest("{\"object_or_name\":\"o\"}");
}

TEST(RpcRequestTest, WrongTapObjectRequest) {
  // Without x
  WrongTapObjectParamsTest("{\"y\":1}");

  // Without y
  WrongTapObjectParamsTest("{\"x\":1}");
}

TEST(RpcRequestTest, CreateIconReloadRequest_ValidId_ValidRequest) {
  uint64_t id = 42;
  auto message = common::jmsg::MessageFactory::SignalConnection::CreateIconReloadRequest(id);

  Json::Value params;
  Json::Value error;
  std::string method;
  common::jmsg::ParseJsonRpcRequest(message, id, method, params, error);

  EXPECT_EQ(id, 42) << "Id must be equal";
  EXPECT_STREQ(method.c_str(), common::jmsg::kReloadIconStorage) << "Method must be equal";
  EXPECT_TRUE(params.empty()) << "No parameters are assumed";
  EXPECT_FALSE(error) << "Error must be empty";
}

TEST(CreateErrorObjectTest, CreateErrorObject_CreateErrorObjectWithEmptyData_CompareSuccess) {
  auto object = common::jmsg::CreateErrorObject(rpc::Error::kInternalError, "message", Json::Value{});

  Json::Value error_data_empty{Json::objectValue};
  error_data_empty["code"] = static_cast<int>(rpc::Error::kInternalError);
  error_data_empty["message"] = "message";

  EXPECT_EQ(object, error_data_empty);
}

TEST(CreateErrorObjectTest, CreateErrorObject_CreateErrorObjectWithData_CompareSuccess) {
  auto object = common::jmsg::CreateErrorObject(rpc::Error::kInternalError, "message", Json::Value{true});

  Json::Value error_data_exist{Json::objectValue};
  error_data_exist["code"] = static_cast<int>(rpc::Error::kInternalError);
  error_data_exist["message"] = "message";
  error_data_exist["data"] = true;

  EXPECT_EQ(object, error_data_exist);
}

TEST(ParseJsonTest, ParseJson_ValidJsonString_SuccessfullyParse) {
  Json::Value value;
  EXPECT_TRUE(common::jmsg::ParseJson(correct_json_string, value));
}

TEST(ParseJsonTest, ParseJson_InvalidString_FailureParse) {
  Json::Value value;
  EXPECT_FALSE(common::jmsg::ParseJson("wrong structure", value));
}

TEST(CheckHeaderTest, CheckHeader_ValidJsonString_CheckHeaderSuccess) {
  Json::Value value;
  common::jmsg::ParseJson(correct_json_string, value);
  EXPECT_TRUE(common::jmsg::CheckHeader(value));
}

TEST(CheckHeaderTest, CheckHeader_InvalidKeys_CheckFailure) {
  Json::Value value;
  common::jmsg::ParseJson(R"({"wrong-id":1,"wrong-jsonrpc":"2.0","wrong-result":true})", value);
  EXPECT_FALSE(common::jmsg::CheckHeader(value));
}

TEST(CheckHeaderTypeTest, CheckHeaderType_ValidJsonString_CheckSuccess) {
  Json::Value value;
  common::jmsg::ParseJson(correct_json_string, value);
  EXPECT_TRUE(common::jmsg::CheckHeaderType(value));
}

TEST(CheckHeaderTypeTest, CheckHeaderType_WrongIdType_CheckFailure) {
  Json::Value wrong_id_type_json;
  common::jmsg::ParseJson(R"({"id":"123","jsonrpc":"2.0","result":true})", wrong_id_type_json);
  EXPECT_FALSE(common::jmsg::CheckHeaderType(wrong_id_type_json));
}

TEST(CheckHeaderTypeTest, CheckHeaderType_WrongRpcVersion_Failur) {
  Json::Value wrong_rpc_version_json;
  common::jmsg::ParseJson(R"({"id":1,"jsonrpc":"wrong","result":true})", wrong_rpc_version_json);
  EXPECT_FALSE(common::jmsg::CheckHeaderType(wrong_rpc_version_json));
}

TEST(ExtractWaitForObjectParamsTest, ExtractWaitForObjectParams_ValidParam_Success) {
  Json::Value params{};
  params[common::jmsg::kName] = "name";
  params[common::jmsg::kSyncVersion] = "sync";
  params[common::jmsg::kSyncBuildVersion] = "build";
  params[common::jmsg::kParentScreen] = "parent";
  params[common::jmsg::kSyncCollectionMode] =
      common::squish::CollectionModeToStr(common::squish::CollectionMode::kNight);
  params[common::jmsg::kTimeoutMsec] = 1000;
  params[common::jmsg::kXTopLeft] = 10;
  params[common::jmsg::kYTopLeft] = 20;
  params[common::jmsg::kXBottomRight] = 70;
  params[common::jmsg::kYBottomRight] = 80;

  common::ObjectDataIdentity object_data_identity;
  std::chrono::milliseconds timeout;
  common::Point top_left;
  common::Point bottom_right;
  Json::Value error{};

  common::jmsg::ExtractWaitForObjectRequestParams(params, object_data_identity, timeout, top_left, bottom_right, error);

  EXPECT_STREQ(object_data_identity.name.c_str(), "name");
  EXPECT_STREQ(object_data_identity.sync_version.c_str(), "sync");
  EXPECT_STREQ(object_data_identity.build_version.c_str(), "build");
  EXPECT_STREQ(object_data_identity.parent_screen.c_str(), "parent");
  EXPECT_EQ(object_data_identity.mode, common::squish::CollectionMode::kNight);
  EXPECT_EQ(timeout, std::chrono::milliseconds{1000});
  EXPECT_EQ(top_left.x, 10);
  EXPECT_EQ(top_left.y, 20);
  EXPECT_EQ(bottom_right.x, 70);
  EXPECT_EQ(bottom_right.y, 80);
  EXPECT_TRUE(error.empty());
}

TEST(ExtractTapObjectRequestParamsTest, ExtractTapObjectRequestParams_ValidParam_Success) {
  Json::Value params{};

  params["x"] = 1;
  params["y"] = 2;
  params["modifier_state"] = static_cast<int>(common::squish::ModifierState::kNone);
  params["button"] = static_cast<int>(common::squish::MouseButton::kLeftButton);

  int x{};
  int y{};
  common::squish::ModifierState modifier;
  common::squish::MouseButton button;
  Json::Value error{};

  common::jmsg::ExtractTapObjectRequestParams(params, x, y, modifier, button, error);

  EXPECT_EQ(x, 1);
  EXPECT_EQ(y, 2);
  EXPECT_EQ(modifier, common::squish::ModifierState::kNone);
  EXPECT_EQ(button, common::squish::MouseButton::kLeftButton);
  EXPECT_TRUE(error.empty());
}

TEST(ExtractTouchAndDragRequestParamsTest, ExtractTouchAndDragRequestParams_ValidParam_Success) {
  Json::Value params{};

  params["object_or_name"] = "object_name";
  params["x"] = 1;
  params["y"] = 2;
  params["dx"] = 10;
  params["dy"] = 20;
  params["modifier_state"] = static_cast<int>(common::squish::ModifierState::kNone);

  std::string name;
  int x{};
  int y{};
  int dx{};
  int dy{};
  common::squish::ModifierState modifier;
  Json::Value error{};

  common::jmsg::ExtractTouchAndDragRequestParams(params, name, x, y, dx, dy, modifier, error);

  EXPECT_STREQ(name.c_str(), "object_name");
  EXPECT_EQ(x, 1);
  EXPECT_EQ(y, 2);
  EXPECT_EQ(dx, 10);
  EXPECT_EQ(dy, 20);
  EXPECT_EQ(modifier, common::squish::ModifierState::kNone);
  EXPECT_TRUE(error.empty());
}

TEST(ExtractTouchAndDragRequestParamsTest, ExtractTouchAndDragRequestParams_WrongParamsType_ExpectNoThrowSuccess) {
  Json::Value params{};

  params["object_or_name"] = 1;
  params["x"] = "wrong_type";
  params["y"] = "wrong_type";
  params["dx"] = "wrong_type";
  params["dy"] = "wrong_type";
  params["modifier_state"] = static_cast<int>(common::squish::ModifierState::kNone);

  std::string name;
  int x{};
  int y{};
  int dx{};
  int dy{};
  common::squish::ModifierState modifier;
  Json::Value error{};

  EXPECT_NO_THROW(common::jmsg::ExtractTouchAndDragRequestParams(params, name, x, y, dx, dy, modifier, error));
  EXPECT_FALSE(error.empty());
}

TEST(ExtractPressAndHoldRequestParamsTest, ExtractPressAndHoldRequestParams_ValidParam_Success) {
  constexpr auto kX = 1, kY = 2;

  Json::Value params{};
  params["x"] = kX;
  params["y"] = kY;

  int x{}, y{};
  Json::Value error{};
  common::jmsg::ExtractPressAndHoldRequestParams(params, x, y, error);

  EXPECT_EQ(x, kX);
  EXPECT_EQ(y, kY);
  EXPECT_TRUE(error.empty());
}

TEST(ExtractPressAndHoldRequestParamsTest, ExtractPressAndHoldRequestParams_InvalidParam_NoThrow) {
  Json::Value params{};
  params["x"] = "invalid";
  params["y"] = "invalid";

  int x{}, y{};
  Json::Value error{};

  EXPECT_NO_THROW(common::jmsg::ExtractPressAndHoldRequestParams(params, x, y, error));
  EXPECT_FALSE(error.empty());
}

TEST(ExtractPressReleaseRequestParamsTest, ExtractPressReleaseRequestParams_ValidParam_Success) {
  constexpr auto kX = 1, kY = 2;

  Json::Value params{};
  params["x"] = kX;
  params["y"] = kY;

  int x{}, y{};
  Json::Value error{};
  common::jmsg::ExtractPressReleaseRequestParams(params, x, y, error);

  EXPECT_EQ(x, kX);
  EXPECT_EQ(y, kY);
  EXPECT_TRUE(error.empty());
}

TEST(ExtractPressReleaseRequestParamsTest, ExtractPressReleaseRequestParams_InvalidParam_NoThrow) {
  Json::Value params{};
  params["x"] = "invalid";
  params["y"] = "invalid";

  int x{}, y{};
  Json::Value error{};

  EXPECT_NO_THROW(common::jmsg::ExtractPressReleaseRequestParams(params, x, y, error));
  EXPECT_FALSE(error.empty());
}

TEST(ExtractDisplayTypeChangedRequestParamsTest, ExtractDisplayTypeChangedRequestParams_ValidParam_Success) {
  Json::Value params;
  params["x"] = 1;
  params["y"] = 2;

  int x{};
  int y{};
  Json::Value error{};

  common::jmsg::ExtractDisplayTypeChangedRequestParams(params, x, y, error);
  EXPECT_EQ(x, 1);
  EXPECT_EQ(y, 2);
  EXPECT_TRUE(error.empty());
}

TEST(ExtractDisplayTypeChangedRequestParamsTest,
     ExtractDisplayTypeChangedRequestParams_WrongParamsType_ExpectNoThrowSuccess) {
  Json::Value params;
  params["x"] = "qwe";
  params["y"] = "qwe";

  int x{};
  int y{};
  Json::Value error{};

  EXPECT_NO_THROW(common::jmsg::ExtractDisplayTypeChangedRequestParams(params, x, y, error));
  EXPECT_FALSE(error.empty());
}

TEST(CheckGetScreenshotResponseTest, CheckGetScreenshotResponse_ValidResponse_Success) {
  auto correct_response = R"({"id":1,"jsonrpc":"2.0","result":true})";
  Json::Value value;
  common::jmsg::ParseJson(correct_response, value);
  EXPECT_TRUE(common::jmsg::CheckGetScreenshotResponse(value));
}

TEST(CheckGetScreenshotResponseTest, CheckGetScreenshotResponse_InvalidResponse_Failure) {
  auto incorrect_response = R"({"id":1,"jsonrpc":"2.0","result":"qwe"})";
  Json::Value value;
  common::jmsg::ParseJson(incorrect_response, value);
  EXPECT_FALSE(common::jmsg::CheckGetScreenshotResponse(value));
}

TEST(ExtractChangeSyncIconDBRequestParamsTest, ExtractChangeSyncIconDBRequestParams_ValidParam_Success) {
  Json::Value params;
  params["sync_version"] = "SYNC3";
  params["sync_build_version"] = "build_1";

  std::string sync_version;
  std::string sync_build_version;
  Json::Value error{true};

  common::jmsg::ExtractChangeSyncIconDBRequestParams(params, sync_version, sync_build_version, error);

  EXPECT_STREQ(sync_version.c_str(), "SYNC3");
  EXPECT_STREQ(sync_build_version.c_str(), "build_1");
}

TEST(ExtractChangeSyncIconDBRequestParamsTest,
     ExtractChangeSyncIconDBRequestParams_WrongParamsType_ExpectNoThrowSuccess) {
  Json::Value params;
  params["sync_version"] = 1;
  params["sync_build_version"] = 1;

  std::string sync_version;
  std::string sync_build_version;
  Json::Value error{true};

  EXPECT_NO_THROW(common::jmsg::ExtractChangeSyncIconDBRequestParams(params, sync_version, sync_build_version, error));
}

TEST(ExtractChangeSyncIconDBRequestParamsTest, ExtractChangeSyncIconDBRequestParams_EmptyStringInParams_Failure) {
  Json::Value params;
  params["sync_version"] = "";
  params["sync_build_version"] = "";

  std::string sync_version;
  std::string sync_build_version;
  Json::Value error{};

  common::jmsg::ExtractChangeSyncIconDBRequestParams(params, sync_version, sync_build_version, error);
  EXPECT_FALSE(error.empty());
}

TEST(ExtractChangeSyncModeRequestParamsTest, ExtractChangeSyncModeRequestParams_ValidParam_Success) {
  Json::Value params;
  params["sync_collection_mode"] = "day_mode";

  std::string sync_collection_mode;
  Json::Value error{};

  common::jmsg::ExtractChangeSyncModeRequestParams(params, sync_collection_mode, error);
  EXPECT_STREQ(sync_collection_mode.c_str(), "day_mode");
  EXPECT_TRUE(error.empty());
}

TEST(ExtractChangeSyncModeRequestParamsTest,
     ExtractChangeSyncModeRequestParams_WrongTypeOfSyncCollectionMode_ExpectNoThrowSuccess) {
  Json::Value params;
  params["sync_collection_mode"] = 1;

  std::string sync_collection_mode;
  Json::Value error{true};

  EXPECT_NO_THROW(common::jmsg::ExtractChangeSyncModeRequestParams(params, sync_collection_mode, error));
}

TEST(ExtractChangeSyncModeRequestParamsTest, ExtractChangeSyncModeRequestParams_EmptyNameOfCollectionMode_Failure) {
  Json::Value params;
  params["sync_collection_mode"] = "";

  std::string sync_collection_mode;
  Json::Value error{};

  common::jmsg::ExtractChangeSyncModeRequestParams(params, sync_collection_mode, error);
  EXPECT_FALSE(error.empty());
}

TEST(ExtractLongPressRequestParamsTest, ExtractLongPressRequestParams_ValidParam_Success) {
  Json::Value params;
  params["x"] = 1;
  params["y"] = 2;
  params["timeout_msec"] = 2000;

  int x, y;
  std::chrono::milliseconds timeout;
  Json::Value error;

  common::jmsg::ExtractLongPressRequestParams(params, x, y, timeout, error);

  EXPECT_EQ(x, 1);
  EXPECT_EQ(y, 2);
  EXPECT_EQ(timeout, std::chrono::milliseconds(2000));
  EXPECT_TRUE(error.empty());
}

TEST(ExtractLongPressRequestParamsTest, ExtractLongPressRequestParams_WrongParamsType_ExpectNoThrowSuccess) {
  Json::Value params;
  params["x"] = "1";
  params["y"] = "2";
  params["timeout_msec"] = "2000";

  int x, y;
  std::chrono::milliseconds timeout;
  Json::Value error;

  EXPECT_NO_THROW(common::jmsg::ExtractLongPressRequestParams(params, x, y, timeout, error));
  EXPECT_FALSE(error.empty());
}

TEST(ExtractGetTextRequestParams, ExtractGetTextRequestParams_ValidParam_Success) {
  Json::Value params;

  params[common::jmsg::kAbscissa] = 10;
  params[common::jmsg::kOrdinate] = 20;

  params[common::jmsg::kAbscissaDrag] = 70;
  params[common::jmsg::kOrdinateDrag] = 80;

  common::Point top_left{}, bottom_right{};
  Json::Value error;

  common::jmsg::ExtractGetTextRequestParams(params, top_left, bottom_right, error);
  EXPECT_TRUE(error.empty());
  EXPECT_EQ(top_left.x, 10);
  EXPECT_EQ(top_left.y, 20);
  EXPECT_EQ(bottom_right.x, 70);
  EXPECT_EQ(bottom_right.y, 80);
}

TEST(ExtractGetTextRequestParams, ExtractGetTextRequestParams_InvalidParam_ErrorInvalidParams) {
  Json::Value params;

  params[common::jmsg::kAbscissa] = "";  // instead of an integer we pass a string

  common::Point top_left{}, bottom_right{};
  Json::Value error;

  common::jmsg::ExtractGetTextRequestParams(params, top_left, bottom_right, error);

  EXPECT_EQ(error[common::jmsg::kErrorCode].asInt(), static_cast<int>(rpc::Error::kInvalidParams));
  EXPECT_STREQ(error[common::jmsg::kErrorMessage].asCString(), "Invalid GetText params");
  EXPECT_TRUE(error[common::jmsg::kErrorData].isNull());
  EXPECT_EQ(top_left.x, 0);
  EXPECT_EQ(top_left.y, 0);
  EXPECT_EQ(bottom_right.x, 0);
  EXPECT_EQ(bottom_right.y, 0);
}

TEST(ExtractGetScreenshotRequestParamsTest, ExtractGetScreenshotRequestParams_ValidParam_Success) {
  Json::Value params;
  params["filename"] = "file.png";
  params["location"] = "screenshots/location";

  std::string screenshot_name;
  std::string screenshot_location;
  Json::Value error;

  common::jmsg::ExtractGetScreenshotParams(params, screenshot_name, screenshot_location, error);

  EXPECT_STREQ(screenshot_name.c_str(), "file.png");
  EXPECT_STREQ(screenshot_location.c_str(), "screenshots/location");
}

TEST(ExtractGetScreenshotParamsTest, ExtractGetScreenshotRequestParams_WrongParamsType_ExpectNoThrowSuccess) {
  Json::Value params;
  params["filename"] = true;
  params["location"] = false;

  std::string screenshot_name;
  std::string screenshot_location;
  Json::Value error;

  EXPECT_NO_THROW(common::jmsg::ExtractGetScreenshotParams(params, screenshot_name, screenshot_location, error));
}

TEST(ExtractGetObjectsDataByPatternParamsTest,
     ExtractGetObjectsDataByPatternParams_WrongParamsType_ExpectThrowSuccess) {
  Json::Value error;
  Json::Value params;
  common::ObjectDataIdentity object_identity;

  params[common::jmsg::kName] = 123;
  params[common::jmsg::kSyncVersion] = false;
  params[common::jmsg::kSyncBuildVersion] = 123;
  params[common::jmsg::kParentScreen] = "main";
  params[common::jmsg::kSyncCollectionMode] = "DAY";

  EXPECT_NO_THROW(common::jmsg::ExtractGetObjectsDataByPatternParams(params, object_identity, error));
  EXPECT_FALSE(error.empty()) << "Error code does not exist.";
  EXPECT_EQ(static_cast<int>(rpc::Error::kInvalidParams), error["code"].asInt()) << "Errors codes does not match.";
}

TEST(ExtractGetObjectsDataByPatternParamsTest, ExtractGetObjectsDataByPatternParams_ValidType_ExpectedParams) {
  Json::Value error;
  Json::Value params;
  common::ObjectDataIdentity object_identity;

  params[common::jmsg::kName] = "name";
  params[common::jmsg::kSyncVersion] = "sync3";
  params[common::jmsg::kSyncBuildVersion] = "build1";
  params[common::jmsg::kParentScreen] = "main";
  params[common::jmsg::kSyncCollectionMode] = "DAY";

  EXPECT_NO_THROW(common::jmsg::ExtractGetObjectsDataByPatternParams(params, object_identity, error));
  EXPECT_EQ(object_identity.name, "name");
  EXPECT_EQ(object_identity.sync_version, "sync3");
  EXPECT_EQ(object_identity.build_version, "build1");
  EXPECT_EQ(object_identity.parent_screen, "main");
  EXPECT_EQ(object_identity.mode, common::squish::CollectionMode::kDay);
  EXPECT_TRUE(error.empty()) << "Error code exist.";
}

TEST(ExtractGetObjectsDataByPatternParamsTest,
     ExtractGetObjectsDataByPatternParamsObjectName_ValidType_ExpectedObjectName) {
  Json::Value error;
  Json::Value params;
  std::string object_name;
  params[common::jmsg::kName] = "test_name";

  EXPECT_NO_THROW(common::jmsg::ExtractGetObjectsDataByPatternParams(params, object_name, error));
  EXPECT_EQ(object_name, "test_name");
  EXPECT_TRUE(error.empty()) << "Error code exist.";
}

TEST(ExtractGetImagesDiscrepancyParamsTest, ExtractGetImagesDiscrepancyParams_InvalidRequestType_InvalidParamsError) {
  Json::Value error;

  std::string path2;
  std::string path1;
  common::Point top_left;
  common::Point bottom_right;

  const auto expected_path2 = 10;
  const auto expected_path1 = "path1";
  common::Point expected_top_left{0, 10};
  common::Point expected_bottom_right{90, 100};

  Json::Value params;
  params[common::jmsg::kIconPathSecond] = expected_path2;
  params[common::jmsg::kIconPathFirst] = expected_path1;
  params[common::jmsg::kXTopLeft] = expected_top_left.x;
  params[common::jmsg::kYTopLeft] = expected_top_left.y;
  params[common::jmsg::kXBottomRight] = expected_bottom_right.x;
  params[common::jmsg::kYBottomRight] = expected_bottom_right.y;

  common::jmsg::ExtractGetImagesDiscrepancyParams(params, path2, path1, top_left, bottom_right, error);
  EXPECT_TRUE(path2.empty()) << "Path to the second image must be empty.";
  EXPECT_EQ(static_cast<int>(rpc::Error::kInvalidParams), error["code"].asInt()) << "Errors codes does not match.";
}

TEST(ExtractGetImagesDiscrepancyParamsTest, ExtractGetImagesDiscrepancyParams_ValidRequest_ExpectedParams) {
  Json::Value error;

  std::string path2;
  std::string path1;
  common::Point top_left;
  common::Point bottom_right;

  const auto expected_path2 = "path2";
  const auto expected_path1 = "path1";
  common::Point expected_top_left{0, 10};
  common::Point expected_bottom_right{90, 100};

  Json::Value params;
  params[common::jmsg::kIconPathSecond] = expected_path2;
  params[common::jmsg::kIconPathFirst] = expected_path1;
  params[common::jmsg::kXTopLeft] = expected_top_left.x;
  params[common::jmsg::kYTopLeft] = expected_top_left.y;
  params[common::jmsg::kXBottomRight] = expected_bottom_right.x;
  params[common::jmsg::kYBottomRight] = expected_bottom_right.y;

  EXPECT_NO_THROW(common::jmsg::ExtractGetImagesDiscrepancyParams(params, path2, path1, top_left, bottom_right, error));
  EXPECT_EQ(path2, expected_path2) << "Path to the second image does not match with expected.";
  EXPECT_EQ(path1, expected_path1) << "Path to the second image does not match with expected.";
  EXPECT_EQ(top_left.x, expected_top_left.x) << "Top left x coordinate does not match with expected.";
  EXPECT_EQ(top_left.y, expected_top_left.y) << "Top left y coordinate does not match with expected.";
  EXPECT_EQ(bottom_right.x, expected_bottom_right.x) << "Bottom right x coordinate does not match with expected.";
  EXPECT_EQ(bottom_right.y, expected_bottom_right.y) << "Bottom right y coordinate does not match with expected.";
  EXPECT_TRUE(error.empty()) << "Error code exist.";
}

TEST(ExtractCaptureFramesParamsTest, ExtractCaptureFramesParams_ValidParams_Valid) {
  Json::Value params;
  params[common::jmsg::kTimeInterval] = 1u;
  params[common::jmsg::kTimeoutMsec] = 100u;
  params[common::jmsg::kXTopLeft] = 10;
  params[common::jmsg::kYTopLeft] = 20;
  params[common::jmsg::kXBottomRight] = 30;
  params[common::jmsg::kYBottomRight] = 40;
  params[common::jmsg::kLocation] = "~/test";

  Json::Value error;
  unsigned int interval{}, duration{};
  common::Point top_left, bottom_right;
  std::string path;

  EXPECT_NO_THROW(
      common::jmsg::ExtractCaptureFramesParams(params, interval, duration, top_left, bottom_right, path, error));
  EXPECT_EQ(interval, 1u);
  EXPECT_EQ(duration, 100u);
  EXPECT_EQ(top_left.x, 10);
  EXPECT_EQ(top_left.y, 20);
  EXPECT_EQ(bottom_right.x, 30);
  EXPECT_EQ(bottom_right.y, 40);
  EXPECT_STREQ(path.c_str(), "~/test");
  EXPECT_TRUE(error.empty()) << "Error code exist";
}

TEST(ExtractCaptureFramesParamsTest, ExtractCaptureFramesParams_InvalidParams_ErrorInvalidParams) {
  Json::Value params;
  params[common::jmsg::kTimeInterval] = "";  // instead of an integer we pass a string

  Json::Value error;
  unsigned int interval{0}, duration{0};
  common::Point top_left, bottom_right;
  std::string path;

  common::jmsg::ExtractCaptureFramesParams(params, interval, duration, top_left, bottom_right, path, error);
  EXPECT_EQ(interval, 0u);
  EXPECT_EQ(duration, 0u);
  EXPECT_EQ(top_left.x, 0);
  EXPECT_EQ(top_left.y, 0);
  EXPECT_EQ(bottom_right.x, 0);
  EXPECT_EQ(bottom_right.y, 0);
  EXPECT_TRUE(path.empty());
  EXPECT_EQ(error[common::jmsg::kErrorCode].asInt(), static_cast<int>(rpc::Error::kInvalidParams));
  EXPECT_STREQ(error[common::jmsg::kErrorMessage].asCString(), "Invalid CaptureFrames params");
  EXPECT_TRUE(error[common::jmsg::kErrorData].isNull());
}

TEST(ExtractFindAllImagesParamsTest, ExtractFindAllImagesParams_ValidParams_Valid) {
  Json::Value params;
  const int kX = 10;
  const int kY = 20;
  const int kWidth = 30;
  const int kHeight = 40;

  params[common::jmsg::kObjectName] = "name";
  params[common::jmsg::kXTopLeft] = kX;
  params[common::jmsg::kYTopLeft] = kY;
  params[common::jmsg::kXBottomRight] = kWidth;
  params[common::jmsg::kYBottomRight] = kHeight;

  Json::Value error;
  std::string object_name;
  common::Point top_left, bottom_right;

  EXPECT_NO_THROW(common::jmsg::ExtractFindAllImagesParams(params, object_name, top_left, bottom_right, error));
  EXPECT_EQ(top_left.x, kX);
  EXPECT_EQ(top_left.y, kY);
  EXPECT_EQ(bottom_right.x, kWidth);
  EXPECT_EQ(bottom_right.y, kHeight);
  EXPECT_TRUE(error.empty()) << "Error code exist";
}

TEST(ExtractFindAllImagesParamsTest, ExtractFindAllImagesParams_EmptyJson_ErrorInvalidParams) {
  Json::Value params;

  params[common::jmsg::kObjectName] = "name";
  params[common::jmsg::kXTopLeft] = "some string";  // string instead of int
  params[common::jmsg::kYTopLeft] = 20;
  params[common::jmsg::kXBottomRight] = 30;
  params[common::jmsg::kYBottomRight] = 40;

  Json::Value error;
  std::string object_name;
  common::Point top_left, bottom_right;

  EXPECT_NO_THROW(common::jmsg::ExtractFindAllImagesParams(params, object_name, top_left, bottom_right, error));
  EXPECT_EQ(error[common::jmsg::kErrorCode].asInt(), static_cast<int>(rpc::Error::kInvalidParams));
}

TEST(ExtractFindAllImagesParamsTest, ExtractFindAllImagesParams_EmptyName_ErrorInvalidParams) {
  Json::Value params;

  params[common::jmsg::kObjectName] = "";
  params[common::jmsg::kXTopLeft] = 10;
  params[common::jmsg::kYTopLeft] = 20;
  params[common::jmsg::kXBottomRight] = 30;
  params[common::jmsg::kYBottomRight] = 40;

  Json::Value error;
  std::string object_name;
  common::Point top_left, bottom_right;

  EXPECT_NO_THROW(common::jmsg::ExtractFindAllImagesParams(params, object_name, top_left, bottom_right, error));
  EXPECT_EQ(error[common::jmsg::kErrorCode].asInt(), static_cast<int>(rpc::Error::kInvalidParams));
}

TEST(ExtractFindAllImagesParamsTest, ExtractFindAllImagesParams_NegativeCoordinates_ErrorInvalidParams) {
  Json::Value params;

  params[common::jmsg::kObjectName] = "";
  params[common::jmsg::kXTopLeft] = 10;
  params[common::jmsg::kYTopLeft] = -1;
  params[common::jmsg::kXBottomRight] = 30;
  params[common::jmsg::kYBottomRight] = 40;

  Json::Value error;
  std::string object_name;
  common::Point top_left, bottom_right;

  EXPECT_NO_THROW(common::jmsg::ExtractFindAllImagesParams(params, object_name, top_left, bottom_right, error));
  EXPECT_EQ(error[common::jmsg::kErrorCode].asInt(), static_cast<int>(rpc::Error::kInvalidParams));
}

}  // namespace
