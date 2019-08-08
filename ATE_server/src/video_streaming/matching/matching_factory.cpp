#include "video_streaming/matching/matching_factory.h"

#include "exceptions.h"
#include "utils/defines.h"
#include "video_streaming/matching/multiscale_template_detector.h"
#include "video_streaming/matching/template_detector.h"

using namespace defines;

namespace detector {

std::unique_ptr<detector::Detector<cv::Mat>> MakeImageDetector(const std::string& detector_type, double confidence) {
  if (detector_type == kTemplateMathcing) {
    return confidence ? std::make_unique<detector::TemplateDetector>(confidence)
                      : std::make_unique<detector::TemplateDetector>(kDefaultImageDetectorConfidence);
  }

  if (detector_type == kMultiscaleTemplateMatchng) {
    return confidence
               ? std::make_unique<detector::MultiscaleTemplateDetector>(confidence)
               : std::make_unique<detector::MultiscaleTemplateDetector>(kDefaultMultiscaleImageDetectorConfidence);
  }

  throw ImageDetectorTypeError{};
}

}  // namespace detector
