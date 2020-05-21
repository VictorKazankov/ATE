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
  auto expected_message = R"({"id":1,"jsonrpc":"2.0","method":"AttachToApplication","params":{"timeout_msec":1000}})";

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
  auto request_message = common::jmsg::MessageFactory::Client::CreateChangeSyncModeRequest("DAY", 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0",
      "method":"ChangeSyncMode",
      "params":{"sync_collection_mode":"DAY"}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateTapObjectRequest_ValidJsonMessage_Success) {
  auto request_message = common::jmsg::MessageFactory::Client::CreateTapObjectRequest(
      1, 1, common::squish::ModifierState::kNone, common::squish::MouseButton::kLeftButton, 1);
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
      "object", 1, 1, 2, 2, common::squish::ModifierState::kNone, 1);
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
  auto request_message =
      common::jmsg::MessageFactory::Client::CreateWaitForObjectRequest("object_name", 1, {10, 20}, {70, 80}, 1);
  auto expected_message =
      R"({"id" :1,"jsonrpc":"2.0",
      "method":"WaitForObject",
      "params":{"name":"object_name",
                "timeout_msec":1, "x_top_left":10,"y_top_left":20,"x_bottom_right":70,"y_bottom_right":80}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateWaitForObjectRequest_PassObjectDataIdentity_ValidJsonMessage) {
  common::ObjectDataIdentity object_data_identity;
  object_data_identity.name = "test_name";
  object_data_identity.sync_version = "test_sync";
  object_data_identity.build_version = "test_build";
  object_data_identity.parent_screen = "test_parent_screen";
  object_data_identity.mode = common::squish::CollectionMode::kNight;

  auto request_message =
      common::jmsg::MessageFactory::Client::CreateWaitForObjectRequest(object_data_identity, 1, {10, 20}, {70, 80}, 1);
  auto expected_message =
      R"({"id" :1,"jsonrpc":"2.0",
      "method":"WaitForObject",
      "params":{"name":"test_name",
                "sync_version":"test_sync",
                "sync_build_version":"test_build",
                "parent_screen":"test_parent_screen",
                "sync_collection_mode":"NIGHT",
                "timeout_msec":1, "x_top_left":10,"y_top_left":20,"x_bottom_right":70,"y_bottom_right":80}})";
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
  auto expected_message = R"({"id":1,"jsonrpc":"2.0","method":"LongPress","params":{"timeout_msec":2000,"x":1,"y":2}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateGetTextRequest_ValidJsonMessage_Success) {
  auto request_message =
      common::jmsg::MessageFactory::Client::CreateGetTextRequest(common::Point{10, 20}, common::Point{70, 80}, 1);
  auto expected_message = R"({"id":1,"jsonrpc":"2.0","method":"GetText","params":{"x":10,"y":20,"dx":70,"dy":80}})";
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

TEST(MessageFactoryClientTest, CreateGetObjectsDataByPatternRequest_InputObjectName_ValidJsonMessage) {
  auto request_message = common::jmsg::MessageFactory::Client::CreateGetObjectsDataByPatternRequest("test_name", 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0","method":"GetObjectsDataByPattern","params":{"name":"test_name"}})";

  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateGetObjectsDataByPatternRequest_InputData_ValidJsonMessage) {
  auto request_message = common::jmsg::MessageFactory::Client::CreateGetObjectsDataByPatternRequest(
      {"test_name", "sync3", "revision1", common::squish::CollectionMode::kDay, "main"}, 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0","method":"GetObjectsDataByPattern","params":{"name":"test_name","parent_screen":"main","sync_build_version":"revision1","sync_collection_mode":"DAY","sync_version":"sync3"}})";

  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateGetImageDiscrepancyRequest_InputData_ValidJsonMessage) {
  auto request_message = common::jmsg::MessageFactory::Client::CreateGetImagesDiscrepancyRequest(
      "/image2.png", "/image1.png", {0, 0}, {100, 100}, 1);
  auto expected_message =
      R"(
        {
          "id":1,"jsonrpc":"2.0",
          "method":"GetImagesDiscrepancy",
          "params":{
            "icon_path_second":"/image2.png",
            "icon_path_first":"/image1.png",
            "x_top_left": 0,
            "y_top_left": 0,
            "x_bottom_right": 100,
            "y_bottom_right": 100
          }
        }
      )";

  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateCaptureFramesRequest_ValidData_ExpectedMessage) {
  auto request_message = common::jmsg::MessageFactory::Client::CreateCaptureFramesRequest(2, 100, common::Point(1, 2),
                                                                                          common::Point(4, 5), ".", 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0","method":"CaptureFrames","params":{"time_interval_msec":2,"timeout_msec":100,
          "x_top_left":1,"y_top_left":2,"x_bottom_right":4,"y_bottom_right":5,"location":"."}})";
  EXPECT_TRUE(JsonComparator(request_message, expected_message))
      << "Request message: " << request_message << " Expected message: " << expected_message;
}

TEST(MessageFactoryClientTest, CreateFindAllImagesRequest_ValidData_ExpectedMessage) {
  auto request_message = common::jmsg::MessageFactory::Client::CreateFindAllImagesRequest(
      "object_under_test", common::Point(1, 2), common::Point(4, 5), 1);
  auto expected_message =
      R"({"id":1,"jsonrpc":"2.0","method":"FindAllImages","params":{"object_or_name":"object_under_test",
          "x_top_left":1,"y_top_left":2,"x_bottom_right":4,"y_bottom_right":5}})";
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
      << "Response message: " << server_response_failure << " Expected response: " << expected_failure_response;
}

TEST(MessageFactoryServerTest, CreateGetObjectsDataByPatternResponse_ValidServerResponse_Success) {
  common::ObjectData object_data;
  object_data.name = "test_name";
  object_data.parent_screen = "test_parent";
  object_data.build_version = "test_build";
  object_data.sync_version = "test_sync";
  object_data.mode = common::squish::CollectionMode::kDay;
  object_data.center = {10, 10};
  object_data.top_left = {5, 15};
  object_data.bottom_right = {15, 5};
  object_data.width = 20;
  object_data.height = 20;
  object_data.parent_width = 1920;
  object_data.parent_height = 1200;
  std::vector<common::ObjectData> object_list{object_data};

  std::string expected_response_str =
      R"([{"x":10, "y":10, "width":20, "height":20, "x_top_left":5, "y_top_left":15, "x_bottom_right":15, "y_bottom_right":5, "parent_width":1920, "parent_height":1200, "name":"test_name", "parent_screen":"test_parent", "sync_version":"test_sync", "sync_build_version":"test_build", "sync_collection_mode":"DAY"}])";
  Json::Value expected_response;
  Json::Reader reader;
  reader.parse(expected_response_str, expected_response);

  auto response = common::jmsg::MessageFactory::Server::CreateGetObjectsDataByPatternResponse(object_list);

  EXPECT_EQ(response, expected_response) << "Incorrect response size.";
  EXPECT_EQ(response.size(), object_list.size()) << "Response from the server does not match with expected.\n"
                                                 << expected_response << "\n"
                                                 << response;
}

TEST(MessageFactoryServerTest, CreateGetImageDiscrepancyResponse_ValidServerResponse_Success) {
  std::string expected_response_str = R"({"discrepancy_percent":10})";

  Json::Value expected_response;
  Json::Reader reader;
  reader.parse(expected_response_str, expected_response);

  auto response = common::jmsg::MessageFactory::Server::CreateGetImagesDiscrepancyResponse(10);
  EXPECT_EQ(response, expected_response) << "Incorrect response.";
}

TEST(MessageFactoryServerTest, CreateCaptureFramesResponse_ListOfFilenames_ValidResponse) {
  std::string expected_str = R"({"filename":["frame1.png","frame2.png","frame3.png"]})";
  Json::Reader reader;
  Json::Value expected;

  reader.parse(expected_str, expected);
  auto response = common::jmsg::MessageFactory::Server::CreateCaptureFramesResponse(
      std::vector<std::string>{"frame1.png", "frame2.png", "frame3.png"});
  EXPECT_EQ(response, expected) << "Response message: " << response.toStyledString()
                                << " Expected response: " << expected_str;
}

TEST(MessageFactoryServerTest, CreateFindAllImagesResponse_ThreeItems_ResponseWithThreeItems) {
  std::vector<common::Rect> response_list{common::Rect(1, 1, 2, 2), common::Rect(2, 2, 2, 2), common::Rect(3, 3, 2, 2)};
  std::string expected_str =
      R"([{"x":1,"y":1,"width":2,"height":2},{"x":2,"y":2,"width":2,"height":2},{"x":3,"y":3,"width":2,"height":2}])";
  Json::Reader reader;
  Json::Value expected;

  reader.parse(expected_str, expected);
  auto response = common::jmsg::MessageFactory::Server::CreateFindAllImagesResponse(response_list);
  EXPECT_EQ(response, expected) << "Response message: " << response.toStyledString()
                                << " Expected response: " << expected_str;
}

}  // namespace
