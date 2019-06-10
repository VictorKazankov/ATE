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
  storage_ = std::make_unique<storage::JsonStorage>(kImageStoragePath);
  storage_->LoadCollection(kGoodImageCollectionFile);
}

void JsonStorageTest::TearDown() { storage_.reset(); }

TEST_F(JsonStorageTest, NoFileExistConstruction) {
  storage::JsonStorage storage("/home");
  EXPECT_FALSE(storage.LoadCollection(kGoodImageCollectionFile));
}

TEST_F(JsonStorageTest, CorruptedFileConstruction) {
  storage::JsonStorage storage(kImageStoragePath);
  EXPECT_FALSE(storage.LoadCollection(kCorruptedImageCollectionFile));
}

TEST_F(JsonStorageTest, GoodFileConstruction) {
  storage::JsonStorage storage(kImageStoragePath);
  EXPECT_TRUE(storage.LoadCollection(kGoodImageCollectionFile));
}

TEST_F(JsonStorageTest, ChangeCollection) {
  EXPECT_TRUE(storage_->LoadCollection(kGoodSpareImageCollectionFile));
  EXPECT_FALSE(storage_->LoadCollection(kCorruptedImageCollectionFile));
}

TEST_F(JsonStorageTest, ChangeCollectionToTheSame) {
  EXPECT_TRUE(storage_->LoadCollection(kGoodImageCollectionFile));
}

TEST_F(JsonStorageTest, GetItemPath) {
  EXPECT_FALSE(storage_->ItemPath(kItemContacts).empty()) << "Storage::ItemPath Must return valid path";
  EXPECT_FALSE(storage_->ItemPath(kItemApps).empty()) << "Storage::ItemPath Must return valid path";
  EXPECT_FALSE(storage_->ItemPath(kItemAudio).empty()) << "Storage::ItemPath Must return valid path";
  EXPECT_TRUE(storage_->ItemPath(kItemBadItem).empty()) << "Storage::ItemPath Must return empty string";
}
}  // namespace
