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

bool Contains(const cv::Rect& rect, const cv::Point& pt) {
  // the code from cv::Rect::contains  x <= pt.x && pt.x < x + width && y <= pt.y && pt.y < y + height;
  // it doesn't take into account the right and bottom edges
  return rect.x <= pt.x && pt.x <= rect.x + rect.width && rect.y <= pt.y && pt.y <= rect.y + rect.height;
}

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

std::error_code Matcher::GetFrame(cv::Mat& frame, const cv::Rect& area) {
  if (!video_status_->GetVideoStatus() || !GrabNewFrame()) {
    return common::make_error_code(common::AteError::kVideoTemporarilyUnavailable);
  }

  const cv::Rect kEmptyRect{};  // standard `empty` method checks width == 0 or height == 0, so in this case can be
                                // bug with points (9999, 9999), (9999, 9999)
  const cv::Rect frame_boundaries{cv::Point{0, 0}, screen_.size()};

  if (area != kEmptyRect) {
    // fix difference in 1 pixel when area and frame have the same rectangle
    if (frame_boundaries != area &&
        (area.empty() || !Contains(frame_boundaries, area.tl()) || !Contains(frame_boundaries, area.br()))) {
      logger::error("[matcher GetFrame] Desired area is out of screen boundaries: screen - {}x{}, area - {}:{} ; {}:{}",
                    frame_boundaries.width, frame_boundaries.height, area.tl().x, area.tl().y, area.br().x,
                    area.br().y);
      return common::make_error_code(common::AteError::kOutOfBoundaries);
    }
  }
  frame = (area.empty() || area == frame_boundaries) ? screen_ : screen_(area);
  return {};
}

std::pair<cv::Rect, std::error_code> Matcher::DetectImage(const std::string& object, const cv::Mat& pattern) {
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
    screenshot_recorder_->TakeScreenshots(screen_, gray_screen_, {detected_object}, object);
  }

  if (detected_object.empty()) {
    return {cv::Rect{}, common::make_error_code(common::AteError::kPatternNotFound)};
  }

  return {detected_object, std::error_code{}};
}

std::pair<cv::Rect, std::error_code> Matcher::DetectText(const std::string& text) {
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
    screenshot_recorder_->TakeScreenshots(screen_, gray_screen_, {detected_area}, text);
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

std::error_code Matcher::GetScreenshot(const std::string& file_name, const std::string& file_path,
                                       const cv::Rect& area) {
  if (!video_status_->GetVideoStatus() || !GrabNewFrame()) {
    logger::error("[matcher] Video stream unavailable");
    return {common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};
  }

  if (!area.empty()) {
    const cv::Rect screen_rect{0, 0, screen_.size().width, screen_.size().height};
    if (!Contains(screen_rect, area.tl()) || !Contains(screen_rect, area.br())) {
      logger::error("[matcher] Desired area is out of screen boundaries");
      return {common::make_error_code(common::AteError::kOutOfBoundaries)};
    }
  }

  utils::ScreenshotError screenshot_error =
      utils::ScreenshotRecorder::GetScreenshot(area.empty() ? screen_ : screen_(area), file_name, file_path);

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

  const cv::Rect crop_area(point, delta_point);
  const cv::Rect screen_rect{cv::Point{0, 0}, screen_.size()};

  if (crop_area.empty() || !Contains(screen_rect, point) || !Contains(screen_rect, delta_point)) {
    logger::error("[matcher] Desired area is out of screen boundaries");
    return {{}, common::make_error_code(common::AteError::kOutOfBoundaries)};
  }

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
  cv::Rect screen_rect{cv::Point{0, 0}, image_second.size()};
  if (!Contains(screen_rect, top_left_coordinate) || !Contains(screen_rect, bottom_right_coordinate)) {
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

std::pair<std::vector<cv::Rect>, std::error_code> Matcher::DetectImages(const std::string& object,
                                                                        const cv::Mat& pattern, const cv::Rect& area) {
  if (!video_status_->GetVideoStatus() || !GrabNewFrame()) {
    logger::error("[matcher] Video stream unavailable");
    return {{}, common::make_error_code(common::AteError::kVideoTemporarilyUnavailable)};
  }

  if (pattern.empty()) {
    logger::error("[matcher] Find pattern is empty");
    return {{}, common::make_error_code(common::AteError::kPatternInvalid)};
  }

  if (pattern.rows > screen_.rows || pattern.cols > screen_.cols) {
    logger::error("[matcher] Wrong pattern for image detection: pattern size: {}, screen size: {}", pattern.size,
                  screen_.size);

    return {{}, common::make_error_code(common::AteError::kPatternInvalid)};
  }

  if (!area.empty()) {
    const cv::Rect screen_rect{0, 0, screen_.size().width, screen_.size().height};
    if (!Contains(screen_rect, area.tl()) || !Contains(screen_rect, area.br())) {
      logger::error("[matcher] Desired area is out of screen boundaries");
      return {{}, common::make_error_code(common::AteError::kOutOfBoundaries)};
    }
  }

  const std::vector<cv::Rect> detected_objects =
      image_detector_->DetectAll(area.empty() ? screen_ : screen_(area), pattern);

  if (screenshot_recorder_) {
    screenshot_recorder_->TakeScreenshots(screen_, gray_screen_, detected_objects, object);
  }

  if (detected_objects.empty()) {
    return {{}, common::make_error_code(common::AteError::kPatternNotFound)};
  }

  return {detected_objects, std::error_code{}};
}

}  // namespace detector
