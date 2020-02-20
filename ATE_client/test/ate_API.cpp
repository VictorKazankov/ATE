#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ate_api.h"
#include "interaction.h"
#include "squish/squish_types.h"

#define private public
#define protected public
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
 public:
  void SetUp() override;
  void TearDown() override;

  std::unique_ptr<squish::ApplicationContext> application_context_;
  std::shared_ptr<MockATEInteraction> mock_{nullptr};
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

TEST_F(AteApiTest, GetObjectsDataByPattern_PassStringNameReceiveVaildResponse_ReceiveValidResponse) {
  std::string valid_response(R"({"id":1,"jsonrpc":"2.0","result":[{"height":10,"width":10,"x":1,"y":1},{"height":10,"width":10,"x":1,"y":1}]})");
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(valid_response));

  API::AteApi api;
  std::vector<squish::Object> objects = api.GetObjectsDataByPattern(mock_, 1, "name");
  EXPECT_TRUE(!objects.empty());

  for (const auto& object : objects) {
    EXPECT_EQ(object.x, 1);
    EXPECT_EQ(object.y, 1);
    EXPECT_EQ(object.width, 10);
    EXPECT_EQ(object.height, 10);
  }
}

TEST_F(AteApiTest, GetObjectsDataByPattern_PassSquishObjectReceiveVaildResponse_ReceiveValidResponse) {
  std::string valid_response(R"({"id":1,"jsonrpc":"2.0","result":[{"height":10,"width":10,"x":1,"y":1},{"height":10,"width":10,"x":1,"y":1}]})");
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(valid_response));

  squish::Object valid_object;
  valid_object.name = "object_name";

  API::AteApi api;
  std::vector<squish::Object> objects = api.GetObjectsDataByPattern(mock_, 1, valid_object);
  EXPECT_TRUE(!objects.empty());

  for (const auto& object : objects) {
    EXPECT_EQ(object.x, 1);
    EXPECT_EQ(object.y, 1);
    EXPECT_EQ(object.width, 10);
    EXPECT_EQ(object.height, 10);
  }
}

TEST_F(AteApiTest, GetObjectsDataByPattern_PassSquishObject_ReceiveResponseWithInvalidResult) {
  std::string invalid_response(R"({"id":1,"jsonrpc":"2.0","result":[{"height":10,"width":10,"y":1}]})");
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(invalid_response));

  squish::Object valid_object;
  valid_object.name = "object_name";

  API::AteApi api;
  std::vector<squish::Object> objects = api.GetObjectsDataByPattern(mock_, 1, valid_object);
  EXPECT_TRUE(objects.empty());
}
