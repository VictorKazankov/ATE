#include "ate.h"

#include <stdexcept>

#include "common.h"
#include "exceptions.h"
#include "interaction/SPI/spi_interaction.h"
#include "interaction/VDP/vdp_interaction.h"
#include "interaction/dummy/dummy_interaction.h"
#include "logger/logger.h"
#include "storage/json_storage.h"
#include "utils/ate_server_app_context.h"
#include "utils/defines.h"

namespace {

defines::DisplayType StrToDisplayType(const std::string& display_type_str) {
  if ("G1_8INCH_DISP" == display_type_str) return defines::DisplayType::G1_8INCH_DISP;
  if ("G2_6INCH_DISP" == display_type_str) return defines::DisplayType::G2_6INCH_DISP;
  if ("G2_8INCH_DISP" == display_type_str) return defines::DisplayType::G2_8INCH_DISP;
  if ("G2_10LINCH_DISP" == display_type_str) return defines::DisplayType::G2_10LINCH_DISP;
  if ("G2_10PINCH_DISP" == display_type_str) return defines::DisplayType::G2_10PINCH_DISP;
  return defines::DisplayType::UNDEFINED_DISP;
}

std::unique_ptr<interaction::Interaction> InteractionFactory(const std::string& interaction_type,
                                                             boost::asio::io_context& io_context) {
  if (interaction_type == defines::kSpi) {
    return std::make_unique<interaction::SpiInteraction>(
        common::Config().GetString(defines::kInteraction, defines::kDeviceAddress, ""),
        StrToDisplayType(common::Config().GetString(defines::kInteraction, defines::kDisplayTypeOption, "")));
  }

  if (interaction_type == defines::kVdp) {
    return std::make_unique<interaction::VDPInteraction>(
        io_context, common::Config().GetString(defines::kInteraction, defines::kAddressOption, ""),
        common::Config().GetString(defines::kInteraction, defines::kPortOption, ""),
        StrToDisplayType(common::Config().GetString(defines::kInteraction, defines::kDisplayTypeOption, "")));
  }

  if (interaction_type == defines::kDummy) {
    logger::critical("[ATE] Used DummyInteraction for debugging only");
    return std::make_unique<interaction::DummyInteraction>();
  }

  logger::critical("[ATE] Undefined type of interaction");
  throw interaction::InteractionTypeError{};
}
}  // namespace

ATE::ATE(const utils::ATEServerAppContext& app_context, boost::asio::io_context& io_context)
    : storage_{std::make_unique<storage::JsonStorage>(app_context.StorageDir())},
      interaction_{InteractionFactory(common::Config().GetString(defines::kInteraction, defines::kInteractionType, ""),
                                      io_context)} {
  if (!storage_->LoadCollection(common::Config().GetString(defines::kDBSection, defines::kCollectionModeOption, {}))) {
    throw storage::ConnectionFailure{};
  }
}

ATE::~ATE() = default;

void ATE::TapObject(const cv::Point& point) { interaction_->Tap(point.x, point.y); }

cv::Rect ATE::WaitForObject(const std::string& object_or_name, const std::chrono::milliseconds& timeout) {
  const auto timeout_point = std::chrono::steady_clock::now() + timeout;
  const auto item_path = storage_->ItemPath(object_or_name);
  const bool is_image = !item_path.empty();
  cv::Rect match_result;

  do {
    match_result = is_image ? matcher_.MatchImage(item_path) : matcher_.MatchText(object_or_name);
  } while (match_result.empty() && std::chrono::steady_clock::now() <= timeout_point);

  return match_result;
}
