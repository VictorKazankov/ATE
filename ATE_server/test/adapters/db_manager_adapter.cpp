#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>

#include <db_manager/icon_data_mapper.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

// Helper macros for testing private section
#define protected public
#define private public
#include "adapters/db_manager_adapter.h"

using ::testing::_;
using ::testing::Invoke;
using testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

using namespace db_manager;
using namespace adapter;

/**
 * @brief Mocking MatchResults
 */
class ResultsMock : public MatchResults {
 public:
  MOCK_METHOD0(Next, IconData());
  MOCK_CONST_METHOD0(HasNext, bool());
};

/**
 * @brief Mocking IconDataMapper
 */
class MockDBManager : public IconDataMapper {
 public:
  MOCK_METHOD1(Get, Icon(const IconIdentity&));
  MOCK_METHOD1(Insert, void(const Icon&));
  MOCK_METHOD1(Delete, void(const IconIdentity&));
  MOCK_METHOD2(Update, void(const IconIdentity&, const Icon&));
  MOCK_METHOD1(Match, std::unique_ptr<MatchResults>(const IconWildcard&));
};

class DBManagerAdapterTest : public ::testing::Test {
 protected:
 protected:
  void SetUp() override;

 public:
  DBManagerError init_result_;
  MockDBManager* db_manager_mock_{nullptr};
  adapter::DBManagerAdapter adapter_;
  DBManagerAdapter::StorageConfig storage_config_{"Sync", "Build", db_manager::HmiMode::kDay};
};

void DBManagerAdapterTest::SetUp() {
  init_result_ = adapter_.Init(std::make_unique<MockDBManager>(), "Sync4", "Build", "Day");
  db_manager_mock_ = dynamic_cast<MockDBManager*>(adapter_.icon_data_mapper_.get());
  assert(db_manager_mock_ != nullptr);
}

TEST_F(DBManagerAdapterTest, Init_CorrectParameters_Success) { EXPECT_EQ(init_result_, DBManagerError::kSuccess); }

TEST_F(DBManagerAdapterTest, ValidateConfiguration_EmptySyncVersion_InvalidSyncVersion) {
  adapter::DBManagerAdapter adapter;
  auto result = adapter_.ValidateConfiguration(DBManagerAdapter::StorageConfig{"", "Build", db_manager::HmiMode::kDay});
  EXPECT_EQ(result, DBManagerError::kInvalidSyncVersion);
}

TEST_F(DBManagerAdapterTest, ValidateConfiguration_EmptySyncBuildVersion_InvalidSyncBuildVersion) {
  auto result = adapter_.ValidateConfiguration(DBManagerAdapter::StorageConfig{"Sync4", "", db_manager::HmiMode::kDay});
  EXPECT_EQ(result, DBManagerError::kInvalidSyncBuildVersion);
}

TEST_F(DBManagerAdapterTest, ValidateConfiguration_IncorrectCollectionMode_InvalidCollectionMode) {
  auto result =
      adapter_.ValidateConfiguration(DBManagerAdapter::StorageConfig{"Sync4", "Build", db_manager::HmiMode::kNone});
  EXPECT_EQ(result, DBManagerError::kInvalidCollectionMode);
}

TEST_F(DBManagerAdapterTest, CheckConfiguration_EmptyDbManagerInstance_LogicError) {
  adapter::DBManagerAdapter adapter;
  auto result = adapter.CheckConfiguration(storage_config_);
  EXPECT_EQ(result, DBManagerError::kLogicError);
}

TEST_F(DBManagerAdapterTest, CheckConfiguration_InvalidMatch_LogicError) {
  auto m = std::make_unique<ResultsMock>();

  EXPECT_CALL(*m, HasNext()).Times(0);
  EXPECT_CALL(*db_manager_mock_, Match(_)).WillOnce(Return(::testing::ByMove(nullptr)));

  auto result = adapter_.CheckConfiguration(storage_config_);
  EXPECT_EQ(result, DBManagerError::kLogicError);
}

TEST_F(DBManagerAdapterTest, CheckConfiguration_ValidConfiguration_Success) {
  auto result_mock = std::make_unique<ResultsMock>();

  EXPECT_CALL(*result_mock, HasNext()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*db_manager_mock_, Match(_)).Times(1).WillOnce(Return(::testing::ByMove(std::move(result_mock))));

  auto result = adapter_.CheckConfiguration(storage_config_);
  EXPECT_EQ(result, DBManagerError::kSuccess);
}

TEST_F(DBManagerAdapterTest, CheckConfiguration_InvalidSyncVersion_InvalidSyncVersion) {
  auto result_mock1 = std::make_unique<ResultsMock>();
  auto result_mock2 = std::make_unique<ResultsMock>();

  EXPECT_CALL(*result_mock1, HasNext()).Times(1).WillOnce(Return(false));
  EXPECT_CALL(*result_mock2, HasNext()).Times(1).WillOnce(Return(false));
  EXPECT_CALL(*db_manager_mock_, Match(_))
      .WillOnce(Return(::testing::ByMove(std::move(result_mock1))))
      .WillOnce(Return(::testing::ByMove(std::move(result_mock2))));

  auto result = adapter_.CheckConfiguration(storage_config_);
  EXPECT_EQ(result, DBManagerError::kInvalidSyncVersion);
}

TEST_F(DBManagerAdapterTest, CheckConfiguration_InvalidSyncBuildVersion_InvalidSyncBuildVersion) {
  auto result_mock1 = std::make_unique<ResultsMock>();
  auto result_mock2 = std::make_unique<ResultsMock>();
  auto result_mock3 = std::make_unique<ResultsMock>();

  EXPECT_CALL(*result_mock1, HasNext()).WillOnce(Return(false));
  EXPECT_CALL(*result_mock2, HasNext()).WillOnce(Return(true));
  EXPECT_CALL(*result_mock3, HasNext()).WillOnce(Return(false));
  EXPECT_CALL(*db_manager_mock_, Match(_))
      .Times(3)
      .WillOnce(Return(::testing::ByMove(std::move(result_mock1))))
      .WillOnce(Return(::testing::ByMove(std::move(result_mock2))))
      .WillOnce(Return(::testing::ByMove(std::move(result_mock3))));

  auto result = adapter_.CheckConfiguration(storage_config_);
  EXPECT_EQ(result, DBManagerError::kInvalidSyncBuildVersion);
}

TEST_F(DBManagerAdapterTest, CheckConfiguration_InvalidCollectionMode_InvalidCollectionMode) {
  auto result_mock1 = std::make_unique<ResultsMock>();
  auto result_mock2 = std::make_unique<ResultsMock>();
  auto result_mock3 = std::make_unique<ResultsMock>();
  auto result_mock4 = std::make_unique<ResultsMock>();

  EXPECT_CALL(*result_mock1, HasNext()).WillOnce(Return(false));
  EXPECT_CALL(*result_mock2, HasNext()).WillOnce(Return(true));
  EXPECT_CALL(*result_mock3, HasNext()).WillOnce(Return(true));
  EXPECT_CALL(*result_mock4, HasNext()).WillOnce(Return(false));
  EXPECT_CALL(*db_manager_mock_, Match(_))
      .Times(4)
      .WillOnce(Return(::testing::ByMove(std::move(result_mock1))))
      .WillOnce(Return(::testing::ByMove(std::move(result_mock2))))
      .WillOnce(Return(::testing::ByMove(std::move(result_mock3))))
      .WillOnce(Return(::testing::ByMove(std::move(result_mock4))));

  auto result = adapter_.CheckConfiguration(storage_config_);
  EXPECT_EQ(result, DBManagerError::kInvalidCollectionMode);
}

TEST_F(DBManagerAdapterTest, ChangeConfiguration_ValidConfiguretion_Success) {
  auto result_mock = std::make_unique<ResultsMock>();

  EXPECT_CALL(*result_mock, HasNext()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*db_manager_mock_, Match(_)).Times(1).WillOnce(Return(::testing::ByMove(std::move(result_mock))));

  auto result = adapter_.ChangeConfiguration(storage_config_);
  EXPECT_EQ(result, DBManagerError::kSuccess);
}

TEST_F(DBManagerAdapterTest, Getitem_EmptyPixmapReturn_EmptyImage) {
  auto result_mock = std::make_unique<ResultsMock>();

  EXPECT_CALL(*result_mock, HasNext()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*db_manager_mock_, Match(_)).Times(1).WillOnce(Return(::testing::ByMove(std::move(result_mock))));

  Icon icon{IconIdentity{"sync4", "build", HmiMode::kDay, "name"}, IconMetadata{Point{0, 0}}, Pixmap{}};
  EXPECT_CALL(*db_manager_mock_, Get(_)).Times(1).WillOnce(Return(icon));

  cv::Mat result_image = adapter_.GetItem("name");

  EXPECT_TRUE(result_image.empty()) << "Returned image must be empty.";
}

TEST_F(DBManagerAdapterTest, Getitem_ValidConfiguration_ExpectedImage) {
  auto result_mock = std::make_unique<ResultsMock>();

  EXPECT_CALL(*result_mock, HasNext()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*db_manager_mock_, Match(_)).Times(1).WillOnce(Return(::testing::ByMove(std::move(result_mock))));

  const auto path_to_screenshot = ATE_SERVER_TEST_DATA_PATH "/video_streaming/matching/matcher_tests_big_image.png";
  cv::Mat expected_image = cv::imread(path_to_screenshot, cv::IMREAD_COLOR);

  Pixmap pixmap;
  std::ifstream file(path_to_screenshot);
  file >> std::noskipws;
  std::copy(std::istream_iterator<char>(file), std::istream_iterator<char>(), std::back_inserter(pixmap));
  Icon icon{IconIdentity{"sync4", "build", HmiMode::kDay, "name"}, IconMetadata{Point{0, 0}}, pixmap};
  EXPECT_CALL(*db_manager_mock_, Get(_)).Times(1).WillOnce(Return(icon));

  cv::Mat result_image = adapter_.GetItem("name");

  bool is_equal = std::equal(result_image.begin<uchar>(), result_image.end<uchar>(), expected_image.begin<uchar>());
  EXPECT_TRUE(is_equal) << "Returned image must be equal to expected.";
}

TEST_F(DBManagerAdapterTest, GetItemDataByWildcard_EmptyObjectArray) {
  auto result_mock = std::make_unique<ResultsMock>();

  EXPECT_CALL(*result_mock, HasNext()).Times(1).WillOnce(Return(false));
  EXPECT_CALL(*db_manager_mock_, Match(_)).Times(1).WillOnce(Return(::testing::ByMove(std::move(result_mock))));

  auto result = adapter_.GetItemDataByWildcard({});

  EXPECT_TRUE(result.empty()) << "Object array must be empty";
}
