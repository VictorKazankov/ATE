#include <fcntl.h>

#include <gtest/gtest.h>

#include "utils/file_descriptor.h"

namespace {

TEST(FileDescriptorTest, OpenSuccess) { EXPECT_NO_THROW(utils::FileDescriptor("/dev/ptmx", O_RDWR)); }

TEST(FileDescriptorTest, SwapSuccess) {
  utils::FileDescriptor desc1("/dev/ptmx", O_RDWR);
  EXPECT_LE(0, desc1.Handle());

  utils::FileDescriptor desc2("/dev/tty", O_RDWR);
  EXPECT_LE(0, desc2.Handle());

  int fd2 = desc2.Handle();
  desc1 = std::move(desc2);
  EXPECT_EQ(fd2, desc1.Handle());

  utils::FileDescriptor desc3(std::move(desc1));
  EXPECT_EQ(fd2, desc3.Handle());
}

}  // namespace
