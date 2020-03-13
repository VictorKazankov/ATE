#include "video_streaming/matching/matcher.h"

#include <cassert>
#include <utility>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ate_error.h"
#include "factory/recognition_factory.h"
#include "logger/logger.h"
#include "utils/video_status.h"
#include "video_streaming/streamer.h"

namespace detector {

Matcher::Matcher(std::unique_ptr<streamer::Streamer> streamer, std::unique_ptr<Detector<cv::Mat>> image_detector,
                 std::unique_ptr<Detector<std::string>> text_detector,
                 std::unique_ptr<utils::ScreenshotRecorder> screenshot_recorder,
                 std::unique_ptr<utils::VideoStatus> video_status)
    : streamer_{std::move(streamer)},
      image_detector_{std::move(image_detector)},
      text_detector_{std::move(text_detector)},
      screenshot_recorder_{std::move(screenshot_recorder)},
      video_status_{std::move(video_status)} {
  assert(streamer_);
  assert(image_detector_);
  assert(text_detector_);
}

Matcher::~Matcher() = default;

bool Matcher::GrabNewFrame() {
  const bool result = streamer_->Frame(screen_);
  if (result) {
    cv::cvtColor(screen_, gray_screen_, cv::COLOR_BGR2GRAY);
  } else {
    // Clear old cache images
    screen_.release();
    gray_screen_.release();

    logger::critical("[matcher] Couldn't grab a frame");
  }
  return result;
}

std::pair<cv::Rect, std::error_code> Matcher::MatchImage(const std::string& object, const cv::Mat& pattern) {
  if (!video_status_->GetVideoStatus() || !GrabNewFrame()) {
    logger::error("[matcher] Video stream unavailable");
    return {cv::Rect{}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};
  }
  if (pattern.empty()) {
    logger::error("[matcher] Find pattern is empty");
    return {cv::Rect{}, common::make_error_code(common::AteError::kPatternInvalid)};
  }
  if (pattern.rows > screen_.rows || pattern.cols > screen_.cols) {
    logger::error("[matcher] Wrong pattern for image detection: pattern size: {}, screen size: {}", pattern.size,
                  screen_.size);

    return {cv::Rect{}, common::make_error_code(common::AteError::kPatternInvalid)};
  }

  const cv::Rect detected_object = image_detector_->Detect(screen_, pattern);

  if (screenshot_recorder_) {
    screenshot_recorder_->TakeScreenshots(screen_, gray_screen_, detected_object, object);
  }

  if (detected_object.empty()) {
    return {cv::Rect{}, common::make_error_code(common::AteError::kPatternNotFound)};
  }

  return {detected_object, std::error_code{}};
}

std::pair<cv::Rect, std::error_code> Matcher::MatchText(const std::string& text) {
  if (text.empty()) {
    logger::error("[matcher] The search text is empty");
    return {cv::Rect{}, common::make_error_code(common::AteError::kPatternInvalid)};
  }

  if (!video_status_->GetVideoStatus() || !GrabNewFrame()) {
    logger::error("[matcher] Video stream unavailable");
    return {cv::Rect{}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};
  }

  cv::Rect detected_area = text_detector_->Detect(screen_, text);

  if (screenshot_recorder_) {
    screenshot_recorder_->TakeScreenshots(screen_, gray_screen_, detected_area, text);
  }

  if (detected_area.empty()) {
    return {cv::Rect{}, common::make_error_code(common::AteError::kPatternNotFound)};
  }

  return {detected_area, std::error_code{}};
}

void Matcher::ChangeResolution(int x, int y) { streamer_->ChangeResolution(x, y); }

void Matcher::ChangePreprocessingList(const std::string& sync_version) {
  auto new_detector_instance = factory::CreateTextDetector(sync_version);
  if (new_detector_instance != nullptr) {
    text_detector_.swap(new_detector_instance);
  } else {
    logger::error("Can't create text detector for sync version: {}", sync_version);
  };
}

std::error_code Matcher::GetScreenshot(const std::string& file_name, const std::string& file_path) {
  if (!video_status_->GetVideoStatus() || !GrabNewFrame()) {
    logger::error("[matcher] Video stream unavailable");
    return {common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};
  }

  utils::ScreenshotError screenshot_error = utils::ScreenshotRecorder::GetScreenshot(screen_, file_name, file_path);

  if (screenshot_error != utils::ScreenshotError::kSuccess) {
    switch (screenshot_error) {
      case utils::ScreenshotError::kEmptyFileName:
        return {common::make_error_code(common::AteError::kEmptyFileName)};

      case utils::ScreenshotError::kWrongExtension:
        return {common::make_error_code(common::AteError::kWrongExtension)};

      case utils::ScreenshotError::kPermissionDenied:
        return {common::make_error_code(common::AteError::kPermissionDenied)};

      case utils::ScreenshotError::kImageAssemblingFailed:
        return {common::make_error_code(common::AteError::kImageAssemblingFailed)};

      default:
        return {common::make_error_code(common::AteError::kSystemError)};
    }
  }

  return std::error_code{};
}

std::pair<std::string, std::error_code> Matcher::GetText(const cv::Point& point, const cv::Point& delta_point) {
  if (!video_status_->GetVideoStatus() || !GrabNewFrame()) {
    logger::error("[matcher] Video stream unavailable");
    return {{}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};
  }

  cv::Rect screen_rect{0, 0, screen_.size().width, screen_.size().height};
  if (!screen_rect.contains(point) || !screen_rect.contains(delta_point)) {
    logger::error("[matcher] Desired area is out of screen boundaries");
    return {{}, common::make_error_code(common::AteError::kOutOfBoundaries)};
  }

  cv::Rect crop_area(point, delta_point);
  cv::Mat croped = screen_(crop_area);

  return {text_detector_->ExtractText(croped), std::error_code{}};
}

std::pair<int, std::error_code> Matcher::GetImagesDiscrepancy(const std::string& icon_path_second,
                                                              const std::string& icon_path_first,
                                                              const cv::Point& top_left_coordinate,
                                                              const cv::Point& bottom_right_coordinate) const {
  int discrepancy = 100;
  std::error_code error{};

  // Read images as grayscale
  cv::Mat image_second = cv::imread(icon_path_second, cv::IMREAD_GRAYSCALE);
  cv::Mat image_first = cv::imread(icon_path_first, cv::IMREAD_GRAYSCALE);

  // Images must not be empty
  if (image_first.empty() || image_second.empty()) {
    logger::error("[GetImagesDiscrepancy] Compared images are empty.");
    error = common::make_error_code(common::AteError::kSystemError);
    return {discrepancy, error};
  }

  // Images resolution must be equal
  if (image_second.rows != image_first.rows || image_second.cols != image_first.cols) {
    logger::error("[matcher GetImagesDiscrepancy] Images must be equal resolution.");
    error = common::make_error_code(common::AteError::kWrongImageResolution);
    return {discrepancy, error};
  }

  // The compared area should be within the resolution of images
  cv::Rect screen_rect{0, 0, image_second.size().width, image_second.size().height};
  if (!screen_rect.contains(top_left_coordinate) || !screen_rect.contains(bottom_right_coordinate)) {
    logger::error("[matcher GetImagesDiscrepancy] Desired area is out of screen boundaries");
    return {discrepancy, common::make_error_code(common::AteError::kOutOfBoundaries)};
  }

  try {
    // Forming images for comparison
    if (top_left_coordinate != bottom_right_coordinate) {
      screen_rect = {top_left_coordinate, bottom_right_coordinate};
    }
    cv::Mat cmp_image_second = image_second(screen_rect);
    cv::Mat cmp_image_first = image_first(screen_rect);

    // Calculate the difference
    cv::Mat diff;
    cv::absdiff(cmp_image_second, cmp_image_first, diff);

    // Count of the discrepancy
    int total_number_of_pixels = diff.rows * diff.cols;
    int non_zero_pixels = countNonZero(diff);
    discrepancy = 100 * non_zero_pixels / total_number_of_pixels;

  } catch (const cv::Exception& exception) {
    logger::error("[GetImagesDiscrepancy] Exception during operation on images: {}", exception.what());
    error = common::make_error_code(common::AteError::kSystemError);
  }

  return {discrepancy, error};
}

}  // namespace detector
