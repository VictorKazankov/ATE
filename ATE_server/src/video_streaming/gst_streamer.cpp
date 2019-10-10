#include "video_streaming/gst_streamer.h"

#include <cassert>

#include <gst/app/gstappsink.h>

#include <opencv2/imgproc.hpp>

#include "logger/logger.h"

namespace {

class GstElementStateGuard {
 public:
  explicit GstElementStateGuard(GstElement* gst_element) noexcept;
  ~GstElementStateGuard() noexcept;

 private:
  GstElement* const gst_element_;
};

GstElementStateGuard::GstElementStateGuard(GstElement* gst_element) noexcept : gst_element_{gst_element} {
  assert(gst_element_);
  gst_element_set_state(gst_element_, GST_STATE_PLAYING);
}

GstElementStateGuard::~GstElementStateGuard() noexcept { gst_element_set_state(gst_element_, GST_STATE_NULL); }

std::string ErrorMessage(const GError& error) {
  std::string error_string = "Domain: ";

  error_string.append(g_quark_to_string(error.domain))
      .append(" (")
      .append(std::to_string(error.domain))
      .append("), Code: ")
      .append(std::to_string(error.code))
      .append(", Message: ")
      .append(error.message);

  return error_string;
}
}  // namespace

namespace streamer {

void GstStreamer::GstElementDeleter::operator()(GstElement* element) const noexcept {
  gst_element_set_state(element, GST_STATE_NULL);
  gst_object_unref(element);
}

GstStreamer::GstStreamer(const std::string& path, const cv::Size& size) : matrix_size_{size} {
  std::string gst_pipeline_config = "shmsrc socket-path=";
  gst_pipeline_config.append(path).append(" ! appsink name=buff_sink");

  gst_init(nullptr, nullptr);

  GError* error = nullptr;
  pipeline_.reset(gst_parse_launch(gst_pipeline_config.c_str(), &error));
  std::unique_ptr<GError, void (*)(GError*)> error_wrapper{error, g_error_free};

  if (error_wrapper) {
    const std::string error_message = ErrorMessage(*error_wrapper);
    // if the pipeline is null, then this is an error, otherwise a warning
    if (!pipeline_) {
      // Error
      std::string error_what = "Error while opening GStreamer device \"";
      error_what.append(path).append("\": ").append(error_message);

      throw std::runtime_error{error_what};
    }
    // Warning
    logger::warn("[gstreamer] {}", error_message);
  }
}

bool GstStreamer::Frame(cv::Mat& frame) {
  const GstElementStateGuard pipeline_state_guard{pipeline_.get()};

  const std::unique_ptr<GstElement, GstElementDeleter> testsink{
      gst_bin_get_by_name(GST_BIN(pipeline_.get()), "buff_sink")};
  if (!testsink) {
    logger::error("[gstreamer] gst_bin_get_by_name return null");
    return false;
  }

  // Timeout 1 seconds
  auto get_sample_timeout = std::chrono::nanoseconds(std::chrono::seconds(1)).count();
  // Trying to get gstreamer sample. If sample not ready after time out - finishing trying getting sample
  const std::unique_ptr<GstSample, void (*)(GstSample*)> sample{
      gst_app_sink_try_pull_sample(GST_APP_SINK(testsink.get()), get_sample_timeout), gst_sample_unref};
  if (!sample) {
    logger::error("[gstreamer] gst_app_sink_pull_sample return null");
    return false;
  }

  GstBuffer* const buffer = gst_sample_get_buffer(sample.get());
  if (!buffer) {
    logger::error("[gstreamer] gst_sample_get_buffer return null");
    return false;
  }

  return WriteGstBufferToCvMat(buffer, frame);
}

void GstStreamer::ChangeResolution(int x, int y) { matrix_size_ = cv::Size{x, y}; }

bool GstStreamer::WriteGstBufferToCvMat(GstBuffer* buffer, cv::Mat& result) const {
  GstMapInfo map_info = {};
  if (!gst_buffer_map(buffer, &map_info, GST_MAP_READ)) {
    logger::error("[gstreamer] gst_buffer_map return false");
    return false;
  }
  try {
    const cv::Mat buffer_mat{matrix_size_, CV_8UC4, map_info.data};
    cv::cvtColor(buffer_mat, result, cv::COLOR_BGRA2BGR);
  } catch (...) {
    gst_buffer_unmap(buffer, &map_info);
    logger::error("[gstreamer] An unexpected error occurs while extracting data from the buffer");
    throw;
  }
  gst_buffer_unmap(buffer, &map_info);
  return true;
}

}  // namespace streamer
