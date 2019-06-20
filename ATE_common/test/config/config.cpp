#include <cmath>
#include <cstdint>
#include <limits>
#include <memory>

#include <gtest/gtest.h>

#include "config/config.h"
#include "exceptions.h"

namespace {

constexpr auto kConfigFilename = VHAT_COMMON_TEST_DATA_PATH "/config/test_config.ini";

class ConfigReaderTest : public ::testing::Test {
 protected:
  void SetUp() override;
  void TearDown() override;

  std::unique_ptr<config::Reader> reader_;
};

void ConfigReaderTest::SetUp() { reader_ = std::make_unique<config::Reader>(kConfigFilename); }

void ConfigReaderTest::TearDown() { reader_.reset(); }

TEST_F(ConfigReaderTest, BadConstruction) {
  EXPECT_THROW(config::Reader{"there_is_no_file"}, config::FileReadFailure);
  EXPECT_THROW(config::Reader{VHAT_COMMON_TEST_DATA_PATH "/config/bad_comment.ini"}, config::FileReadFailure);
}

TEST_F(ConfigReaderTest, SecondaryConstruction) { EXPECT_NO_THROW(config::Reader{kConfigFilename}); }

TEST_F(ConfigReaderTest, ParseBool) {
  const std::string section = "TestBool";

  EXPECT_FALSE(reader_->GetBool(section, "Off", true));
  EXPECT_TRUE(reader_->GetBool(section, "On", false));

  EXPECT_FALSE(reader_->GetBool(section, "False", true));
  EXPECT_TRUE(reader_->GetBool(section, "True", false));

  EXPECT_FALSE(reader_->GetBool(section, "No", true));
  EXPECT_TRUE(reader_->GetBool(section, "Yes", false));

  EXPECT_FALSE(reader_->GetBool(section, "NumberZero", true));
  EXPECT_TRUE(reader_->GetBool(section, "NumberOne", false));
}

TEST_F(ConfigReaderTest, ParseInt) {
  static_assert(sizeof(int) == 4, "Check if this test case is correct");

  const std::string section = "TestInt";
  EXPECT_EQ(reader_->GetInt(section, "Zero", 1), 0);
  EXPECT_EQ(reader_->GetInt(section, "One", 0), 1);

  EXPECT_EQ(reader_->GetInt(section, "MinInt32", 0), std::numeric_limits<std::int32_t>::min());
  EXPECT_EQ(reader_->GetInt(section, "MaxInt32", 0), std::numeric_limits<std::int32_t>::max());

  //TODO: FIXME Test is fail in Window. MinInt32Minus1 = 2147483648 might not true in Win
  // Return a default value if the number does not fall into the bounds
  const std::string min_int32_minus1 = "MinInt32Minus1";
  EXPECT_EQ(reader_->GetInt(section, min_int32_minus1, 0), 0);
  EXPECT_EQ(reader_->GetInt(section, min_int32_minus1, 1), 1);

  //TODO: FIXME Test is fail in Window. 
  const std::string max_int32_plus1 = "MaxInt32Plus1";
  EXPECT_EQ(reader_->GetInt(section, max_int32_plus1, 0), 0);
  EXPECT_EQ(reader_->GetInt(section, max_int32_plus1, 1), 1);

  EXPECT_EQ(reader_->GetInt(section, "Oct", 0), 8);
  EXPECT_EQ(reader_->GetInt(section, "OctZero", 1), 0);
  EXPECT_EQ(reader_->GetInt(section, "OctNegative", 0), -9);

  EXPECT_EQ(reader_->GetInt(section, "Hex", 0), 16);
  EXPECT_EQ(reader_->GetInt(section, "HexZero", 1), 0);
  EXPECT_EQ(reader_->GetInt(section, "HexNegative", 0), -17);
}

TEST_F(ConfigReaderTest, ParseDouble) {
  const std::string section = "TestDouble";

  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "Int", 0.), 1.);
  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "IntNegative", 0.), -1.);

  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "Simple", 0.), 1.2);
  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "SimpleNegative", 0.), -3.4);

  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "DotEnded", 0.), 1.);
  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "DotEndedNegative", 0.), -1.);

  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "DigitEnded", 0.), 1.);
  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "DigitEndedNegative", 0.), -1.);

  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "Exponent", 0.), 1.02e+004);
  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "ExponentNegative", 0.), -3.05e+007);

  const double expect_zero = reader_->GetDouble(section, "Zero", 1.);
  EXPECT_DOUBLE_EQ(expect_zero, 0.);
  EXPECT_FALSE(std::signbit(expect_zero));

  const double expect_zero_negative = reader_->GetDouble(section, "ZeroNegative", 1.);
  EXPECT_DOUBLE_EQ(expect_zero_negative, -0.);
  EXPECT_TRUE(std::signbit(expect_zero_negative));

  EXPECT_TRUE(std::isnan(reader_->GetDouble(section, "NaN", 0.)));

  const double expect_inf = reader_->GetDouble(section, "Inf", -0.);
  EXPECT_TRUE(std::isinf(expect_inf));
  EXPECT_FALSE(std::signbit(expect_inf));

  const double expect_inf_negative = reader_->GetDouble(section, "InfNegative", 0.);
  EXPECT_TRUE(std::isinf(expect_inf_negative));
  EXPECT_TRUE(std::signbit(expect_inf_negative));

  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "Hex", 0.), 0x10.1p0);
  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "HexNegative", 0.), -0x10.1p0);

  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "HexExponent", 0.), 0x10p005);
  EXPECT_DOUBLE_EQ(reader_->GetDouble(section, "HexExponentNegative", 0.), -0x23p005);
}

TEST_F(ConfigReaderTest, ParseString) {
  const std::string section = "TestString";
  const std::string empty_str;

  EXPECT_EQ(reader_->GetString(section, "Empty", "NonEmpty"), empty_str);
  EXPECT_EQ(reader_->GetString(section, "WithWhitespaces", empty_str), "WithWhitespaces");
  EXPECT_EQ(reader_->GetString(section, "LowerCase", empty_str), "vhat");
  EXPECT_EQ(reader_->GetString(section, "UpperCase", empty_str), "VHAT");
}

}  // namespace
