#include <gtest/gtest.h>

#include "message_factory/json_defines.h"
#include "message_factory/json_messages.h"
#include "message_factory/json_utils.h"
#include "message_factory/message_factory.h"
#include "rpc_error.h"

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
  std::string object_or_name;
  std::chrono::milliseconds timeout;
  Json::Value error;

  common::jmsg::ExtractWaitForObjectRequestParams(message, object_or_name, timeout, error);

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
  common::squish::ModifierState modifier_state = common::squish::ModifierState::NONE;
  common::squish::MouseButton mouse_button = common::squish::MouseButton::NONE;
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
  params["object_or_name"] = "name";
  params["timeout_msec"] = 1000;

  std::string object_name;
  std::chrono::milliseconds timeout;
  Json::Value error{};

  common::jmsg::ExtractWaitForObjectRequestParams(params, object_name, timeout, error);

  EXPECT_STREQ(object_name.c_str(), "name");
  EXPECT_EQ(timeout, std::chrono::milliseconds{1000});
  EXPECT_TRUE(error.empty());
}

TEST(ExtractTapObjectRequestParamsTest, ExtractTapObjectRequestParams_ValidParam_Success) {
  Json::Value params{};

  params["x"] = 1;
  params["y"] = 2;
  params["modifier_state"] = static_cast<int>(common::squish::ModifierState::NONE);
  params["button"] = static_cast<int>(common::squish::MouseButton::LEFT_BUTTON);

  int x{};
  int y{};
  common::squish::ModifierState modifier;
  common::squish::MouseButton button;
  Json::Value error{};

  common::jmsg::ExtractTapObjectRequestParams(params, x, y, modifier, button, error);

  EXPECT_EQ(x, 1);
  EXPECT_EQ(y, 2);
  EXPECT_EQ(modifier, common::squish::ModifierState::NONE);
  EXPECT_EQ(button, common::squish::MouseButton::LEFT_BUTTON);
  EXPECT_TRUE(error.empty());
}

TEST(ExtractTouchAndDragRequestParamsTest, ExtractTouchAndDragRequestParams_ValidParam_Success) {
  Json::Value params{};

  params["object_or_name"] = "object_name";
  params["x"] = 1;
  params["y"] = 2;
  params["dx"] = 10;
  params["dy"] = 20;
  params["modifier_state"] = static_cast<int>(common::squish::ModifierState::NONE);

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
  EXPECT_EQ(modifier, common::squish::ModifierState::NONE);
  EXPECT_TRUE(error.empty());
}

TEST(ExtractTouchAndDragRequestParamsTest, ExtractTouchAndDragRequestParams_WrongParamsType_ExpectNoThrowSuccess) {
  Json::Value params{};

  params["object_or_name"] = 1;
  params["x"] = "wrong_type";
  params["y"] = "wrong_type";
  params["dx"] = "wrong_type";
  params["dy"] = "wrong_type";
  params["modifier_state"] = static_cast<int>(common::squish::ModifierState::NONE);

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

TEST(CheckWaitForObjectResponseTest, CheckWaitForObjectResponse_ValidResponse_Success) {
  auto correct_response = R"({"id":1,"jsonrpc":"2.0","result":{"height":10,"width":20,"x":1,"y":2}})";

  Json::Value value;
  common::jmsg::ParseJson(correct_response, value);

  EXPECT_TRUE(common::jmsg::CheckWaitForObjectResponse(value));
}

TEST(CheckWaitForObjectResponseTest, CheckWaitForObjectResponse_WrongParamsType_Failure) {
  constexpr auto wrong_param_type =
      R"({"id":1,"jsonrpc":"2.0","result":{"height":"wrong","width":"wrong","x":"wrong","y":"wrong"}})";
  Json::Value value;

  common::jmsg::ParseJson(wrong_param_type, value);
  EXPECT_FALSE(common::jmsg::CheckWaitForObjectResponse(value));
}

TEST(CheckWaitForObjectResponseTest, CheckWaitForObjectResponse_WrongKeyValues_Failure) {
  constexpr auto wrong_key =
      R"({"id":1,"jsonrpc":"2.0","result":{"height":"wrong","width":"wrong","xx":"wrong","yy":"wrong"}})";
  Json::Value value;

  common::jmsg::ParseJson(wrong_key, value);
  EXPECT_FALSE(common::jmsg::CheckWaitForObjectResponse(value));
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

  uint16_t x;
  uint16_t y;
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

  uint16_t x;
  uint16_t y;
  std::chrono::milliseconds timeout;
  Json::Value error;

  EXPECT_NO_THROW(common::jmsg::ExtractLongPressRequestParams(params, x, y, timeout, error));
  EXPECT_FALSE(error.empty());
}

}  // namespace
