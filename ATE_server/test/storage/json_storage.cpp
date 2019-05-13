#include <gtest/gtest.h>

#include "storage/json_storage.h"

namespace {

const std::string kImageStoragePath = VHAT_SERVER_TEST_DATA_PATH "/storage";
const std::string kGoodImageCollectionFile = "test_image_storage.json";
const std::string kGoodSpareImageCollectionFile = "test_image_storage_spare.json";
const std::string kCorruptedImageCollectionFile = "corrupted.json";

const std::string kItemContacts = "contacts";
const std::string kItemApps = "apps";
const std::string kItemAudio = "audio";
const std::string kItemBadItem = "bad_item";

class JsonStorageTest : public ::testing::Test {
 protected:
  void SetUp() override;
  void TearDown() override;

  std::unique_ptr<storage::Storage> storage_;
};

void JsonStorageTest::SetUp() {
  storage_ = std::make_unique<storage::JsonStorage>(kImageStoragePath, kGoodImageCollectionFile);
  storage_->Connect();
}

void JsonStorageTest::TearDown() { storage_.reset(); }

TEST_F(JsonStorageTest, NoFileExistConstruction) {
  storage::JsonStorage storage("/home", "some_folder.json");
  EXPECT_FALSE(storage.Connect());
}

TEST_F(JsonStorageTest, CorruptedFileConstruction) {
  storage::JsonStorage storage(kImageStoragePath, kCorruptedImageCollectionFile);
  EXPECT_FALSE(storage.Connect());
}

TEST_F(JsonStorageTest, GoodFileConstruction) {
  storage::JsonStorage storage(kImageStoragePath, kGoodImageCollectionFile);
  EXPECT_TRUE(storage.Connect());
}

TEST_F(JsonStorageTest, ChangeCollection) {
  EXPECT_TRUE(storage_->ChangeCollection(kGoodSpareImageCollectionFile));
  EXPECT_FALSE(storage_->ChangeCollection(kCorruptedImageCollectionFile));
}

TEST_F(JsonStorageTest, ChangeCollectionToTheSame) {
  EXPECT_FALSE(storage_->ChangeCollection(kGoodImageCollectionFile));
}

TEST_F(JsonStorageTest, GetItemPath) {
  EXPECT_NO_THROW(storage_->ItemPath(kItemContacts));
  EXPECT_NO_THROW(storage_->ItemPath(kItemApps));
  EXPECT_NO_THROW(storage_->ItemPath(kItemAudio));
  EXPECT_THROW(storage_->ItemPath(kItemBadItem), std::runtime_error);
}
}  // namespace
