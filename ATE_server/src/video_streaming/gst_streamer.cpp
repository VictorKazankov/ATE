#include "video_streaming/gst_streamer.h"

#include <cassert>

#include <gst/app/gstappsink.h>

#include <opencv2/imgproc.hpp>

#include "logger/logger.h"

namespace {

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

void GstStreamer::ResizeCvMat(cv::Mat& mat) {
  if (matrix_size_.width == mat.cols && matrix_size_.height == mat.rows && CV_8UC4 == mat.type()) {
    return;
  }
  mat = cv::Mat{matrix_size_, CV_8UC4};

  logger::info("[gstreamer] setting a new format for image: width={}, height={}, type={}", mat.cols, mat.rows,
               mat.type());
}

GstStreamer::GstStreamer(const std::string& path, const cv::Size& size) : buffer_{size, CV_8UC4}, matrix_size_{size} {
  std::string gst_pipeline_config = "shmsrc socket-path=";
  gst_pipeline_config.append(path).append(" ! appsink name=buff_sink");

  gst_init(nullptr, nullptr);

  GError* error = nullptr;
  pipeline_.reset(gst_parse_launch(gst_pipeline_config.c_str(), &error));
  std::unique_ptr<GError, void (*)(GError*)> error_wrapper{error, g_error_free};

  if (error_wrapper) {
    // Avoid using default pipeline
    pipeline_.reset();

    std::string error_message = "Error while opening GStreamer device \"";
    error_message.append(path).append("\": ").append(ErrorMessage(*error_wrapper));

    throw std::runtime_error{error_message};
  }

  assert(pipeline_);
}

bool GstStreamer::Frame(cv::Mat& frame) {
  gst_element_set_state(pipeline_.get(), GST_STATE_PLAYING);

  const std::unique_ptr<GstElement, GstElementDeleter> testsink{
      gst_bin_get_by_name(GST_BIN(pipeline_.get()), "buff_sink")};
  if (!testsink) {
    logger::error("[gstreamer] gst_bin_get_by_name return null");
    return false;
  }

  const std::unique_ptr<GstSample, void (*)(GstSample*)> sample{gst_app_sink_pull_sample(GST_APP_SINK(testsink.get())),
                                                                gst_sample_unref};
  if (!testsink) {
    logger::error("[gstreamer] gst_app_sink_pull_sample return null");
    gst_element_set_state(pipeline_.get(), GST_STATE_NULL);
    return false;
  }

  GstBuffer* const buffer = gst_sample_get_buffer(sample.get());
  if (!buffer) {
    logger::error("[gstreamer] gst_sample_get_buffer return null");
    gst_element_set_state(pipeline_.get(), GST_STATE_NULL);
    return false;
  }

  const auto size = gst_buffer_get_size(buffer);

  ResizeCvMat(frame);
  const auto bytes_extracted = gst_buffer_extract(buffer, 0, static_cast<void*>(buffer_.data), size);
  if (bytes_extracted != size) {
    logger::warn("[gstreamer] Extracted differn count of bytes ({}) than expected ({})", bytes_extracted, size);
    gst_element_set_state(pipeline_.get(), GST_STATE_NULL);
    return false;
  }

  cv::cvtColor(buffer_, frame, cv::COLOR_BGRA2BGR);

  // Todo: Add assertion for the right type of matrix
  // assert(size == frame.rows * frame.cols * frame.channels() * frame.step1());

  gst_element_set_state(pipeline_.get(), GST_STATE_NULL);

  return true;
}

}  // namespace streamer
