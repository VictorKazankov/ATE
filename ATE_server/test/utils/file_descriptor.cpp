#include <fcntl.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "utils/file_descriptor.h"
namespace {

using namespace std;
using ::testing::Test;

class FileDescriptorTest : public Test {
 protected:
  void SetUp() override;
  void TearDown() override;

  const char num_of_file_ = 2;
  vector<string> file_paths_;
};

void FileDescriptorTest::SetUp() {
  for (char i = 0; i < num_of_file_; ++i) {
    char filename[] = "/tmp/fileXXXXXX";
    int fd = mkstemp(filename);
    if (fd != -1) {
      file_paths_.emplace_back(filename);
      close(fd);
    } else {
      perror("File can not be created");
      FAIL() << "Can not create " << filename;
    }
  }
}

void FileDescriptorTest::TearDown() {
  for (auto it : file_paths_) {
    EXPECT_EQ(remove(it.c_str()), 0);
  }

  file_paths_.clear();
}

TEST_F(FileDescriptorTest, OpenSuccess) { EXPECT_NO_THROW(utils::FileDescriptor(file_paths_[0], O_RDWR)); }

TEST_F(FileDescriptorTest, SwapSuccess) {
  utils::FileDescriptor desc1(file_paths_[0], O_RDWR);
  EXPECT_LE(0, desc1.Handle());

  utils::FileDescriptor desc2(file_paths_[1], O_RDWR);
  EXPECT_LE(0, desc2.Handle());

  int fd1 = desc1.Handle();
  int fd2 = desc2.Handle();

  utils::FileDescriptor tmp(std::move(desc1));
  desc1 = std::move(desc2);
  desc2 = std::move(tmp);

  EXPECT_EQ(fd1, desc2.Handle());
  EXPECT_EQ(fd2, desc1.Handle());
}

}  // namespace
