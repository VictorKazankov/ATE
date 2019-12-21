#include "factory/recognition_factory.h"

#include <algorithm>

#include <recognition/factory.h>

#include "common.h"
#include "utils/defines.h"
#include "logger/logger.h"

namespace factory {

static constexpr auto kSync3{"SYNC3"};
static constexpr auto kSync4{"SYNC4"};
static constexpr auto kSync3DefaultPreprocessingList{"Binarized+BinarizedOTSU+None"};
static constexpr auto kSync4DefaultPreprocessingList{"BinarizedCrop+Binarized+BinarizedRGB"};

std::unique_ptr<detector::Detector<std::string>> CreateTextDetector(const std::string& sync_version) {
  // Get data from config
  auto tess_data = common::Config().GetString(defines::kTextDetectorSection, defines::kTessDataOption, {});
  auto preprocessing_list =
      common::Config().GetString(defines::kTextDetectorSection, defines::kTextDetectorPreprocessingList, {});
  auto confidence = common::Config().GetDouble(defines::kTextDetectorSection, defines::kTextDetectorConfidenceOption,
                                               defines::kDefaultTextDetectorConfidence);

  // If not exist preprocessing list in settings - use default preprocessing list
  if (preprocessing_list.empty() && !sync_version.empty()) {
    // Match sync version
    auto case_insensitive_searcher = [](const std::string& str, const std::string& pattern) {
      auto it = std::search(std::begin(str), std::end(str), std::begin(pattern), std::end(pattern),
                            [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); });
      return (it != str.end());
    };

    if (case_insensitive_searcher(sync_version, kSync3)) {
      preprocessing_list = kSync3DefaultPreprocessingList;
    } else if (case_insensitive_searcher(sync_version, kSync4)) {
      preprocessing_list = kSync4DefaultPreprocessingList;
    }
  }
  logger::info("Use next preprocessing list: '{}' for text detection.", preprocessing_list);

  return detector::MakeTextDetector(tess_data, preprocessing_list, confidence);
}

}  // namespace factory
