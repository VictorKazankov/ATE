#include "message_factory/json_defines.h"
#include "message_factory/json_utils.h"
#include "rpc_error.h"

#include <gtest/gtest.h>

namespace {

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
  Json::Value error;

  common::jmsg::ExtractTapObjectRequestParams(message, x, y, error);

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

  // Too big int
  WrongRequestTest("{\"jsonrpc\":\"2.0\",\"id\":18446744073709551616,\"method\":\"m\"}", rpc::Error::kInvalidRequest);

  // String
  WrongRequestTest("{\"jsonrpc\":\"2.0\",\"id\":\"1\",\"method\":\"m\"}", rpc::Error::kInvalidRequest);

  // Float
  WrongRequestTest("{\"jsonrpc\":\"2.0\",\"id\":0.9,\"method\":\"m\"}", rpc::Error::kInvalidRequest);

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
}  // namespace
