#include "message_factory/message_factory.h"

#include <gtest/gtest.h>

#include "json/reader.h"
#include "json/value.h"
#include "json/writer.h"
#include "message_factory/json_defines.h"

namespace {

bool JsonComparator(const std::string& message1, const std::string& message2) {
  Json::Value root_json1;
  Json::Value root_json2;

  Json::Reader reader;

  if (!reader.parse(message1.c_str(), root_json1)) {
    return false;
  }
  if (!reader.parse(message2.c_str(), root_json2)) {
    return false;
  }

  return root_json1 == root_json2;
}

TEST(MessageFactoryClientTest, CreateAttachToApplicationRequest_ValidJsonMessage_Success) {
  auto request_message = common::jmsg::MessageFactory::Client::CreateAttachToApplicationRequest(1000, 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0","method":"AttachToApplication","params":{"timeout_msec":1000}})";

  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateChangeSyncIconDBRequest_ValidJsonMessage_Success) {
  auto request_message =
      common::jmsg::MessageFactory::Client::CreateChangeSyncIconDBRequest("sync_version", "build_version", 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0",
      "method":"ChangeSyncIconDB",
      "params":{"sync_build_version":"build_version",
                "sync_version":"sync_version"}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateChangeSyncModeRequest_ValidJsonMessage_Success) {
  auto request_message = common::jmsg::MessageFactory::Client::CreateChangeSyncModeRequest("day_mode", 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0",
      "method":"ChangeSyncMode",
      "params":{"sync_collection_mode":"day_mode"}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateTapObjectRequest_ValidJsonMessage_Success) {
  auto request_message = common::jmsg::MessageFactory::Client::CreateTapObjectRequest(
      1, 1, common::squish::ModifierState::NONE, common::squish::MouseButton::LEFT_BUTTON, 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0",
      "method":"TapObject",
      "params":{"button":1,
                "modifier_state":0,
                "x":1,
                "y":1}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateTouchAndDragRequest_ValidJsonMessage_Success) {
  auto request_message = common::jmsg::MessageFactory::Client::CreateTouchAndDragRequest(
      "object", 1, 1, 2, 2, common::squish::ModifierState::NONE, 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0",
      "method":"TouchAndDrag",
      "params":{"dx":2,
                "dy":2,
                "modifier_state":0,
                "object_or_name":"object",
                "x":1,
                "y":1}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreatePressAndHoldRequest_ValidJsonMessage_Success) {
  auto request_message = common::jmsg::MessageFactory::Client::CreatePressAndHoldRequest(1, 2, 1);
  auto expected_message = R"({"id": 1,"jsonrpc": "2.0", "method": "PressAndHold", "params" :{"x": 1, "y": 2}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreatePressReleaseRequest_ValidJsonMessage_Success) {
  auto request_message = common::jmsg::MessageFactory::Client::CreatePressReleaseRequest(1, 2, 1);
  auto expected_message = R"({"id": 1,"jsonrpc": "2.0", "method": "PressRelease", "params" :{"x": 1, "y": 2}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateWaitForObjectRequest_ValidJsonMessage_Success) {
  auto request_message = common::jmsg::MessageFactory::Client::CreateWaitForObjectRequest("object_name", 1, 1);
  auto expected_message =
      R"({"id" :1,"jsonrpc":"2.0",
      "method":"WaitForObject",
      "params":{"object_or_name":"object_name",
                "timeout_msec":1}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryDBusConnectionTest, CreateDisplayTypeChangedRequest_ValidJsonMessage_Success) {
  auto request_message = common::jmsg::MessageFactory::DBusConnection::CreateDisplayTypeChangedRequest(1920, 1200, 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0",
      "method":"displayTypeChanged",
      "params":{"x":1920,
                "y":1200}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateLongPressRequest_ValidJsonMessage_Success) {
  auto request_message = common::jmsg::MessageFactory::Client::CreateLongPressRequest(1, 2, 2000, 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0","method":"LongPress","params":{"timeout_msec":2000,"x":1,"y":2}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateGetTextRequest_ValidJsonMessage_Success) {
  auto request_message =
      common::jmsg::MessageFactory::Client::CreateGetTextRequest(common::Point{10, 20}, common::Point{70, 80}, 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0","method":"GetText","params":{"x":10,"y":20,"dx":70,"dy":80}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateGetScreenshotRequest_ValidJsonMessage_Success) {
  auto request_message =
      common::jmsg::MessageFactory::Client::CreateGetScreenshotRequest("filename.png", "location", 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0","method":"GetScreenshot","params":{"filename":"filename.png","location":"location"}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateGetObjectsDataByPatternRequest_InputData_ValidJsonMessage) {
  auto request_message = common::jmsg::MessageFactory::Client::CreateGetObjectsDataByPatternRequest("test_name", 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0","method":"GetObjectsDataByPattern","params":{"select_pattern":"test_name"}})";

  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryDBusConnectionTest, CreateDisplayTypeChangedResponse_ValidResponse_Success) {
  auto response = common::jmsg::MessageFactory::DBusConnection::CreateDisplayTypeChangedResponse();
  EXPECT_EQ(Json::Value{true}, response);
}

TEST(MessageFactoryServerTest, CreateLongPressResultObject_ValidResponse_Success) {
  auto response = common::jmsg::MessageFactory::Server::CreateLongPressResultObject();
  EXPECT_EQ(Json::Value{true}, response);
}

TEST(MessageFactoryServerTest, CreateGetTextResultObject_ValidResponse_Success) {
  const std::string kControlText = "test_text";
  auto response = common::jmsg::MessageFactory::Server::CreateGetTextResultObject(kControlText);

  Json::Value result{Json::objectValue};
  result[common::jmsg::kText] = kControlText;
  EXPECT_EQ(result, response);
}

TEST(MessageFactoryServerTest, CreateTapObjectResultObject_ValidResponse_Success) {
  auto response = common::jmsg::MessageFactory::Server::CreateTapObjectResultObject();
  EXPECT_EQ(Json::Value{true}, response);
}

TEST(MessageFactoryServerTest, CreateTouchAndDragResultObject_ValidResponse_Success) {
  auto response = common::jmsg::MessageFactory::Server::CreateTouchAndDragResultObject();
  EXPECT_EQ(Json::Value{true}, response);
}

TEST(MessageFactoryServerTest, CreatePressAndHoldResultObject_ValidResponse_Success) {
  auto response = common::jmsg::MessageFactory::Server::CreatePressAndHoldResultObject();
  EXPECT_EQ(Json::Value{true}, response);
}

TEST(MessageFactoryServerTest, CreatePressReleaseResultObject_ValidResponse_Success) {
  auto response = common::jmsg::MessageFactory::Server::CreatePressReleaseResultObject();
  EXPECT_EQ(Json::Value{true}, response);
}

TEST(MessageFactoryServerTest, CreateChangeSyncModeResultObject_ValidResponse_Success) {
  auto response = common::jmsg::MessageFactory::Server::CreateChangeSyncModeResultObject();
  EXPECT_EQ(Json::Value{true}, response);
}

TEST(MessageFactoryServerTest, CreateChangeSyncIconDBResultObject_ValidResponse_Success) {
  auto response = common::jmsg::MessageFactory::Server::CreateChangeSyncIconDBResultObject();
  EXPECT_EQ(Json::Value{true}, response);
}

TEST(MessageFactoryServerTest, CreateWaitForObjectResultObject_ValidResponse_Success) {
  auto response = common::jmsg::MessageFactory::Server::CreateWaitForObjectResultObject(1, 2, 3, 4);
  Json::Value result{Json::objectValue};
  result["x"] = 1;
  result["y"] = 2;
  result["width"] = 3;
  result["height"] = 4;

  EXPECT_EQ(response, result);
}

TEST(MessageFactoryServerTest, CreateGetScreenResultObject_ValidResponse_Success) {
  auto response = common::jmsg::MessageFactory::Server::CreateGetScreenshotObject();
  EXPECT_EQ(Json::Value{true}, response);
}

TEST(MessageFactoryServerTest, CreateResponse_ValidServerResponse_Success) {
  auto server_response_success = common::jmsg::MessageFactory::Server::CreateResponse(1, Json::Value{true}, true);
  auto expected_success_response = R"({"id":1,"jsonrpc":"2.0","result":true})";
  EXPECT_TRUE(JsonComparator(server_response_success, expected_success_response))
      << "Response message: " << server_response_success << " Expected response: " << expected_success_response;
}

TEST(MessageFactoryServerTest, CreateResponse_InvalidServerResponse_Failure) {
  auto server_response_failure = common::jmsg::MessageFactory::Server::CreateResponse(0, Json::Value{false}, false);
  auto expected_failure_response = R"({"error":false,"id":null,"jsonrpc":"2.0"})";
  EXPECT_TRUE(JsonComparator(server_response_failure, expected_failure_response))
      << "Response mesasge: " << server_response_failure << " Expected response: " << expected_failure_response;
}

TEST(MessageFactoryServerTest, CreateGetObjectsDataByPatternResponse_ValidServerResponse_Success) {
  common::ObjectData object_data{{10, 10}, {5, 15}, {15, 5}, 20, 20, 1920, 1200, "test_name"};
  std::vector<common::ObjectData> object_list{object_data};

  std::string expected_response_str =
      R"([{"x":10, "y":10, "width":20, "height":20, "x_top_left":5, "y_top_left":15, "x_bottom_right":15, "y_bottom_right":5, "parent_width":1920, "parent_height":1200, "name":"test_name"}])";
  Json::Value expected_response;
  Json::Reader reader;
  reader.parse(expected_response_str, expected_response);

  auto response = common::jmsg::MessageFactory::Server::CreateGetObjectsDataByPatternResponse(object_list);

  EXPECT_EQ(response, expected_response) << "Incorrect response size.";
  EXPECT_EQ(response.size(), object_list.size()) << "Response from the server does not match with expected.\n"
                                                 << expected_response << "\n"
                                                 << response;
}

}  // namespace
