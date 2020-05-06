#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ate_api.h"
#include "interaction.h"
#include "squish/squish_types.h"
#include "squish/wildcard.h"

#include "squish/application_context.h"

using ::testing::_;
using ::testing::Return;

class MockATEInteraction : public interaction::Interaction {
 public:
  MOCK_CONST_METHOD0(host, std::string());
  MOCK_CONST_METHOD0(port, std::string());
  MOCK_METHOD0(Connect, void());
  MOCK_CONST_METHOD0(IsConnectionOpened, bool());
  MOCK_METHOD1(SendCommand, std::string(const std::string&));
};

class AteApiTest : public ::testing::Test {
 protected:
  void SetUp() override;
  void TearDown() override;

  std::unique_ptr<squish::ApplicationContext> application_context_;
  std::shared_ptr<MockATEInteraction> mock_{nullptr};
  API::AteApi api_;
  const uint64_t correlation_id_{1};
  const std::string sync_version_{"sync3"};
  const std::string sync_build_version_{"build_3"};
};

void AteApiTest::SetUp() {
  application_context_ = std::make_unique<squish::ApplicationContext>();
  auto mock = std::make_unique<MockATEInteraction>();
  application_context_->Attach(std::move(mock));
  mock_ = std::make_shared<MockATEInteraction>();
  ASSERT_TRUE(mock_);
}

void AteApiTest::TearDown() {
  application_context_.reset();
  mock_ = nullptr;
}

TEST_F(AteApiTest, GetObjectsDataByPattern_PassObjectName_ReceiveValidResponse) {
  const std::string object_at_json(R"({"sync_collection_mode":"DAY","sync_build_version":"build_3",
  "sync_version":"sync3","parent_screen":"parent_screen_name", "name":"test_name","height":10,"width":10,
  "x":1,"y":1,"x_top_left":5, "y_top_left":5, "x_bottom_right":15, "y_bottom_right":15,
  "parent_width":20, "parent_height":20})");
  const std::string valid_response(R"({"id":1,"jsonrpc":"2.0","result":[)" + object_at_json + "," + object_at_json +
                                   R"(]})");
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(valid_response));

  std::vector<squish::Object> objects = api_.GetObjectsDataByPattern(mock_, 1, "test_name");

  EXPECT_TRUE(!objects.empty());
  for (const auto& object : objects) {
    EXPECT_EQ(object.x, 1);
    EXPECT_EQ(object.y, 1);
    EXPECT_EQ(object.width, 10);
    EXPECT_EQ(object.height, 10);
    EXPECT_EQ(object.name, "test_name");
    EXPECT_EQ(object.parent_screen, "parent_screen_name");
    EXPECT_EQ(object.sync_version, "sync3");
    EXPECT_EQ(object.build_version, "build_3");
    EXPECT_EQ(object.mode, common::squish::CollectionMode::kDay);
    EXPECT_EQ(object.parent_height, 20);
    EXPECT_EQ(object.parent_width, 20);
    EXPECT_EQ(object.x_top_left, 5);
    EXPECT_EQ(object.y_top_left, 5);
    EXPECT_EQ(object.x_bottom_right, 15);
    EXPECT_EQ(object.y_bottom_right, 15);
  }
}

TEST_F(AteApiTest, GetObjectsDataByPattern_PassSquishObjectReceiveVaildResponse_ReceiveValidResponse) {
  const std::string object_at_json(R"({"sync_collection_mode":"DAY","sync_build_version":"build_3",
  "sync_version":"sync3","parent_screen":"parent_screen_name", "name":"name","height":10,"width":10,
  "x":1,"y":1,"x_top_left":5, "y_top_left":5, "x_bottom_right":15, "y_bottom_right":15,
  "parent_width":20, "parent_height":20})");
  const std::string valid_response(R"({"id":1,"jsonrpc":"2.0","result":[)" + object_at_json + "," + object_at_json +
                                   R"(]})");
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(valid_response));

  const squish::Wildcard valid_object("object_name");
  const int kObjectsCountAtResponce = 2;

  std::vector<squish::Object> objects = api_.GetObjectsDataByPattern(mock_, 1, valid_object);
  EXPECT_TRUE(!objects.empty());
  EXPECT_EQ(objects.size(), kObjectsCountAtResponce);

  for (const auto& object : objects) {
    EXPECT_EQ(object.x, 1);
    EXPECT_EQ(object.y, 1);
    EXPECT_EQ(object.width, 10);
    EXPECT_EQ(object.height, 10);
    EXPECT_EQ(object.name, "name");
    EXPECT_EQ(object.parent_screen, "parent_screen_name");
    EXPECT_EQ(object.sync_version, "sync3");
    EXPECT_EQ(object.build_version, "build_3");
    EXPECT_EQ(object.mode, common::squish::CollectionMode::kDay);
    EXPECT_EQ(object.parent_height, 20);
    EXPECT_EQ(object.parent_width, 20);
    EXPECT_EQ(object.x_top_left, 5);
    EXPECT_EQ(object.y_top_left, 5);
    EXPECT_EQ(object.x_bottom_right, 15);
    EXPECT_EQ(object.y_bottom_right, 15);
  }
}

TEST_F(AteApiTest, GetObjectsDataByPattern_PassSquishObject_ReceiveResponseWithInvalidResult) {
  std::string invalid_response(R"({"id":1,"jsonrpc":"2.0","result":[{"height":10,"width":10,"y":1}]})");
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(invalid_response));

  squish::Wildcard valid_object("object_name");

  std::vector<squish::Object> objects = api_.GetObjectsDataByPattern(mock_, 1, valid_object);
  EXPECT_TRUE(objects.empty());
}

TEST_F(AteApiTest, GetImagesDiscrepancy_ValidResponse_ExpectedResult) {
  int expected_discrepancy = 20;
  char valid_response[256]{};
  snprintf(valid_response, sizeof(valid_response) / sizeof(char),
           R"({"id":1,"jsonrpc":"2.0","result":{"discrepancy_percent":%d}})", expected_discrepancy);

  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(valid_response));

  auto discrepancy = api_.GetImagesDiscrepancy(mock_, 1, "path1", "path2", {0, 0}, {100, 100});
  EXPECT_EQ(discrepancy, expected_discrepancy);
}

TEST_F(AteApiTest, CaptureFrames_PassValidParams_ValidResponse) {
  std::string valid_response(R"({"id":1,"jsonrpc":"2.0","result": {"filename":["file1.png","file2.png"]} })");

  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(valid_response));

  auto file_list = api_.CaptureFrames(mock_, 1, 1, 50, common::Point(), common::Point(), ".");

  EXPECT_EQ(file_list.size(), 2) << "expected the list of two filenames";
}

TEST_F(AteApiTest, ChangeSyncIconDB_PassErrorParam_RuntimeError) {
  std::string response(R"({"id":1,"jsonrpc":"2.0",
      "method":"ChangeSyncIconDB",
      "params":{"sync_build_version":"build_version",
                "sync_version":"sync_version"},"error":{"code":-32700}})");
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(response));
  EXPECT_THROW(api_.ChangeSyncIconDB(mock_, correlation_id_, sync_version_, sync_build_version_), std::runtime_error);
}

TEST_F(AteApiTest, ChangeSyncMode_PassErrorParam_RuntimeError) {
  std::string response(R"({"id":1,"jsonrpc":"2.0",
      "method":"ChangeSyncMode",
      "params":{"sync_collection_mode":"DAY"},"error":{"code":-32700}})");
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(response));
  EXPECT_THROW(api_.ChangeSyncMode(mock_, correlation_id_, common::squish::CollectionMode::kDay), std::runtime_error);
}

TEST_F(AteApiTest, GetScreenshot_PassErrorParam_RuntimeError) {
  std::string response(R"({"id":1,"jsonrpc":"2.0","method":"GetScreenshot",
            "params":{"filename":"filename.png",
            "location":"location"},"error":{"code":-32700}})");
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(response));
  EXPECT_THROW(api_.GetScreenshot(mock_, correlation_id_, "filename.png", "location"), std::runtime_error);
}

TEST_F(AteApiTest, GetText_PassErrorParam_RuntimeError) {
  std::string response(R"({"id":1,"jsonrpc":"2.0","method":"GetText",
    "params":{"x":10,"y":20,"dx":70,"dy":80},"error":{"code":-32700}})");
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(response));
  EXPECT_THROW(api_.GetText(mock_, correlation_id_, 10, 20, 70, 80), std::runtime_error);
}
