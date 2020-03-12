#include <gtest/gtest.h>

#include "json_rpc_parser.h"

namespace {

TEST(ExtractGetImagesDiscrepancyResponseParamsTest,
     ExtractGetImagesDiscrepancyResponseParams_ValidRequest_ExpectedParams) {
  int expected_discrepancy = 10;
  char request[256]{};
  snprintf(request, sizeof(request) / sizeof(char), R"({"id":1,"jsonrpc":"2.0","result":{"discrepancy_percent":%d}})",
           expected_discrepancy);
  auto discrepancy = interaction::JsonRpcParser::ParseGetImagesDiscrepancy(request);
  EXPECT_EQ(discrepancy, expected_discrepancy) << "The result does not match the expected value.";
}

TEST(ExtractGetImagesDiscrepancyResponseParamsTest,
     ExtractGetImagesDiscrepancyResponseParams_InvalidRequest_RuntimeError) {
  const auto request = R"({"id":1,"jsonrpc":"2.0","result":{"invalid_name":10}})";
  EXPECT_THROW(interaction::JsonRpcParser::ParseGetImagesDiscrepancy(request), std::runtime_error);
}

}  // namespace
