#include "ate_api.h"

#include "message_factory/message_factory.h"
#include "utils/squish_types_converter.h"

#include "json_rpc_parser.h"

using namespace API;

void AteApi::ChangeSyncIconDB(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                              const uint64_t& correlation_id, const std::string& sync_version,
                              const std::string& sync_build_version) const {
  auto message = common::jmsg::MessageFactory::Client::CreateChangeSyncIconDBRequest(sync_version, sync_build_version,
                                                                                     correlation_id);

  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
}

void AteApi::ChangeSyncMode(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                            const uint64_t& correlation_id, common::squish::CollectionMode collection_mode) const {
  auto message = common::jmsg::MessageFactory::Client::CreateChangeSyncModeRequest(CollectionModeToStr(collection_mode),
                                                                                   correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
}

bool AteApi::GetScreenshot(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                           const uint64_t& correlation_id, const std::string& filename,
                           const std::string& location) const {
  const auto message =
      common::jmsg::MessageFactory::Client::CreateGetScreenshotRequest(filename, location, correlation_id);
  const auto response = ate_interaction->SendCommand(message);

  return interaction::JsonRpcParser::ParseGetScreenshot(response);
}

std::string AteApi::GetText(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                            const uint64_t& correlation_id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) const {
  auto message = common::jmsg::MessageFactory::Client::CreateGetTextRequest(common::Point{x1, y1},
                                                                            common::Point{x2, y2}, correlation_id);

  auto response = ate_interaction->SendCommand(message);
  return interaction::JsonRpcParser::ParseGetText(response);
}

std::vector<squish::Object> AteApi::GetObjectsDataByPattern(
    const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
    const std::string& object_name) const {
  auto message =
      common::jmsg::MessageFactory::Client::CreateGetObjectsDataByPatternRequest(object_name, correlation_id);
  auto response = ate_interaction->SendCommand(message);
  return interaction::JsonRpcParser::ParseGetObjectsDataByPattern(response);
}

std::vector<squish::Object> AteApi::GetObjectsDataByPattern(
    const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
    const squish::Wildcard& wildcard) const {
  auto message = common::jmsg::MessageFactory::Client::CreateGetObjectsDataByPatternRequest(wildcard, correlation_id);

  auto response = ate_interaction->SendCommand(message);
  return interaction::JsonRpcParser::ParseGetObjectsDataByPattern(response);
}

int AteApi::GetImagesDiscrepancy(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                                 const uint64_t& correlation_id, const std::string& icon_path_second,
                                 const std::string& icon_path_first, const common::Point& top_left_coordinate,
                                 const common::Point& bottom_right_coordinate) const {
  auto message = common::jmsg::MessageFactory::Client::CreateGetImagesDiscrepancyRequest(
      icon_path_second, icon_path_first, top_left_coordinate, bottom_right_coordinate, correlation_id);

  auto response = ate_interaction->SendCommand(message);
  return interaction::JsonRpcParser::ParseGetImagesDiscrepancy(response);
}

std::vector<std::string> AteApi::CaptureFrames(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                                               const uint64_t& correlation_id, int interval, int duration,
                                               const common::Point& top_left, const common::Point& bottom_right,
                                               const std::string& path) const {
  auto message = common::jmsg::MessageFactory::Client::CreateCaptureFramesRequest(interval, duration, top_left,
                                                                                  bottom_right, path, correlation_id);
  auto response = ate_interaction->SendCommand(message);
  return interaction::JsonRpcParser::ParseCaptureFrames(response);
}

std::vector<squish::Object> AteApi::FindAllImages(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                                                  const uint64_t& correlation_id, const std::string& object_name,
                                                  const common::Point& top_left, const common::Point& bottom_right) {
  auto message = common::jmsg::MessageFactory::Client::CreateFindAllImagesRequest(object_name, top_left, bottom_right,
                                                                                  correlation_id);
  auto response = ate_interaction->SendCommand(message);
  return interaction::JsonRpcParser::ParseFindAllImages(response);
}
