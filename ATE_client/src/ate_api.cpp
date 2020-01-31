#include "ate_api.h"

#include "message_factory/message_factory.h"
#include "utils/squish_types_converter.h"

#include "json_rpc_parser.h"

using namespace API;

void AteApi::ChangeSyncIconDB(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                              const uint64_t& correlation_id, const std::string& sync_version,
                              const std::string& sync_build_version) {
  auto message = common::jmsg::MessageFactory::Client::CreateChangeSyncIconDBRequest(sync_version, sync_build_version,
                                                                                     correlation_id);

  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
}

void AteApi::ChangeSyncMode(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                            const uint64_t& correlation_id, common::squish::CollectionMode collection_mode) {
  auto message = common::jmsg::MessageFactory::Client::CreateChangeSyncModeRequest(CollectionModeToStr(collection_mode),
                                                                                   correlation_id);
  const auto response = ate_interaction->SendCommand(message);
  interaction::JsonRpcParser::CheckAndRaiseExceptionInCaseErrors(response);
}


bool API::GetScreenshot(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                        const std::string& filename, const std::string& location) {
  const auto message =
      common::jmsg::MessageFactory::Client::CreateGetScreenshotRequest(filename, location, GetCorrelationId());
  const auto response = ate_interaction->SendCommand(message);

  return interaction::JsonRpcParser::ParseGetScreenshot(response);
}

std::string API::GetText(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, uint16_t x1, uint16_t y1,
                         uint16_t x2, uint16_t y2) {
  auto message = common::jmsg::MessageFactory::Client::CreateGetTextRequest(common::Point{x1, y1},
                                                                            common::Point{x2, y2}, GetCorrelationId());

  auto response = ate_interaction->SendCommand(message);
  return interaction::JsonRpcParser::ParseGetText(response);
}
