#include <stdexcept>

#include <gtest/gtest.h>
#include <boost/asio/io_context.hpp>

#include "ate_interaction.h"

namespace {
TEST(ATEInteractionTest, ATEInteraction_InvalidHostOrPort_RuntimeErrorException) {
  const std::string kHost = "invalid_host";
  const std::string kPort = "invalid_port";
  auto io_context = std::make_shared<boost::asio::io_context>();
  EXPECT_THROW(interaction::ATEInteraction(io_context, kHost, kPort), std::runtime_error);
}

TEST(ATEInteractionTest, ATEInteraction_NullIoContext_RuntimeErrorException) {
  const std::string kHost = "localhost";
  const std::string kPort = "65535";
  auto io_context = std::make_shared<boost::asio::io_context>();
  EXPECT_THROW(interaction::ATEInteraction(io_context, kHost, kPort), std::runtime_error);
}
}  // namespace
