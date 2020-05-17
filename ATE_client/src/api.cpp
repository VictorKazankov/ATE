#include "api.h"

#include "api_aggregator.h"

namespace {
API::ApiAggregator& GetApiAggregator() {
  static API::ApiAggregator aggregator;
  return aggregator;
}
}  // namespace

squish::ApplicationContext& API::AttachToApplication(const std::string& aut_name) {
  return GetApiAggregator().AttachToApplication(aut_name);
}

squish::Object API::WaitForObject(const std::string& object_or_name, const common::Point& top_left,
                                  const common::Point& bottom_right) {
  return GetApiAggregator().WaitForObject(object_or_name, top_left, bottom_right);
}

squish::Object API::WaitForObject(const squish::Object& object_or_name, const common::Point& top_left,
                                  const common::Point& bottom_right) {
  return GetApiAggregator().WaitForObject(object_or_name, top_left, bottom_right);
}

squish::Object API::WaitForObject(const std::string& object_or_name, int timeout_msec, const common::Point& top_left,
                                  const common::Point& bottom_right) {
  return GetApiAggregator().WaitForObject(object_or_name, timeout_msec, top_left, bottom_right);
}

squish::Object API::WaitForObject(const squish::Object& object_or_name, int timeout_msec, const common::Point& top_left,
                                  const common::Point& bottom_right) {
  return GetApiAggregator().WaitForObject(object_or_name, timeout_msec, top_left, bottom_right);
}

squish::Object API::WaitForObject(const squish::Wildcard& wildcard, const common::Point& top_left,
                                  const common::Point& bottom_right) {
  return GetApiAggregator().WaitForObject(wildcard, top_left, bottom_right);
}

squish::Object API::WaitForObject(const squish::Wildcard& wildcard, int timeout_msec, const common::Point& top_left,
                                  const common::Point& bottom_right) {
  return GetApiAggregator().WaitForObject(wildcard, timeout_msec, top_left, bottom_right);
}

void API::TapObject(const common::Point& screen_point, common::squish::ModifierState modifier_state,
                    common::squish::MouseButton button) {
  GetApiAggregator().TapObject(screen_point, modifier_state, button);
}

void API::TapObject(const common::Rect& screen_rectangle, common::squish::ModifierState modifier_state,
                    common::squish::MouseButton button) {
  GetApiAggregator().TapObject(screen_rectangle, modifier_state, button);
}

void API::TapObject(const squish::Object& screen_rectangle, common::squish::ModifierState modifier_state,
                    common::squish::MouseButton button) {
  GetApiAggregator().TapObject(screen_rectangle, modifier_state, button);
}

void API::LongPress(const squish::Object& screen_rectangle, int timeout_msec) {
  GetApiAggregator().LongPress(screen_rectangle, timeout_msec);
}

void API::LongPress(const squish::Object& screen_rectangle, int x, int y, int timeout_msec) {
  GetApiAggregator().LongPress(screen_rectangle, x, y, timeout_msec);
}

void API::TouchAndDrag(const squish::Object& object_or_name, int x, int y, int dx, int dy,
                       common::squish::ModifierState modifier_state) {
  GetApiAggregator().TouchAndDrag(object_or_name, x, y, dx, dy, modifier_state);
}

void API::TouchAndDrag(const std::string& object_or_name, int x, int y, int dx, int dy,
                       common::squish::ModifierState modifier_state) {
  GetApiAggregator().TouchAndDrag(object_or_name, x, y, dx, dy, modifier_state);
}

void API::PressAndHold(const common::Point& screen_point) { GetApiAggregator().PressAndHold(screen_point); }

void API::PressAndHold(const common::Rect& screen_rectangle) { GetApiAggregator().PressAndHold(screen_rectangle); }

void API::PressAndHold(const squish::Object& object) { GetApiAggregator().PressAndHold(object); }

void API::PressRelease(const common::Point& screen_point) { GetApiAggregator().PressRelease(screen_point); }

void API::PressRelease(const common::Rect& screen_rectangle) { GetApiAggregator().PressRelease(screen_rectangle); }

void API::PressRelease(const squish::Object& object) { GetApiAggregator().PressRelease(object); }

bool API::Exists(const std::string& object_name, const common::Point& top_left, const common::Point& bottom_right) {
  return GetApiAggregator().Exists(object_name, top_left, bottom_right);
}

void API::ChangeSyncIconDB(const std::string& sync_version, const std::string& sync_build_version) {
  GetApiAggregator().ChangeSyncIconDB(sync_version, sync_build_version);
}

void API::ChangeSyncMode(common::squish::CollectionMode collection_mode) {
  GetApiAggregator().ChangeSyncMode(collection_mode);
}

bool API::GetScreenshot(const std::string& filename, const std::string& location) {
  return GetApiAggregator().GetScreenshot(filename, location);
}

std::string API::GetText(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  return GetApiAggregator().GetText(x1, y1, x2, y2);
}

std::vector<squish::Object> API::GetObjectsDataByPattern(const std::string& object_name) {
  return GetApiAggregator().GetObjectsDataByPattern(object_name);
}

std::vector<squish::Object> API::GetObjectsDataByPattern(const squish::Wildcard& wildcard) {
  return GetApiAggregator().GetObjectsDataByPattern(wildcard);
}

int API::GetImagesDiscrepancy(const std::string& icon_path_second, const std::string& icon_path_first,
                              const common::Point& top_left_coordinate, const common::Point& bottom_right_coordinate) {
  return GetApiAggregator().GetImagesDiscrepancy(icon_path_second, icon_path_first, top_left_coordinate,
                                                 bottom_right_coordinate);
}

std::vector<std::string> API::CaptureFrames(int interval, int duration, const common::Point& top_left,
                                            const common::Point& bottom_right, const std::string& path) {
  return GetApiAggregator().CaptureFrames(interval, duration, top_left, bottom_right, path);
}

std::vector<squish::Object> API::FindAllImages(const std::string& object_name, const common::Point& top_left,
                                               const common::Point& bottom_right) {
  return GetApiAggregator().FindAllImages(object_name, top_left, bottom_right);
}
