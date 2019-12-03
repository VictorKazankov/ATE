#include <gtest/gtest.h>
#include <boost/asio/io_context.hpp>

#define private public
#define protected public
#include "ate_interaction.h"

namespace {
TEST(ATEInteractionTest, ATEInteraction_InvalidArguments_ThrowException) {
  const std::string kHort = "invalid_host";
  const std::string kPort = "invalid_port";
  boost::asio::io_context io_context;
  EXPECT_ANY_THROW(interaction::ATEInteraction(io_context, kHort, kPort));
}
}  // namespace
