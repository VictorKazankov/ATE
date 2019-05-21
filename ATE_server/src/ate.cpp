#include "ate.h"

#include "common.h"
#include "exceptions.h"
#include "interaction/VDP/vdp_interaction.h"
#include "logger/logger.h"
#include "storage/json_storage.h"
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
}  // namespace

ATE::ATE(boost::asio::io_context& io_context)
    : storage_{std::make_unique<storage::JsonStorage>(
          common::Config().GetString(defines::kDBSection, defines::kPathOption, ""),
          common::Config().GetString(defines::kDBSection, defines::kCollectionModeOption, ""))},
      interaction_{std::make_unique<interaction::VDPInteraction>(
          io_context, common::Config().GetString(defines::kBoardSection, defines::kAddressOption, ""),
          common::Config().GetString(defines::kBoardSection, defines::kPortOption, ""),
          StrToDisplayType(common::Config().GetString(defines::kBoardSection, defines::kDisplayTypeOption, "")))} {
  if (!storage_->Connect()) throw storage::ConnectionFailure{};
}

ATE::~ATE() { Close(); }

cv::Rect ATE::waitForObject(const std::string& name, const std::chrono::milliseconds& timeout) {
  const auto item_path = storage_->ItemPath(name);
  Snooze(timeout);
  if (!item_path.empty()) {
    return matcher_.MatchImage(item_path);
  } else {
    return matcher_.MatchText(name);
  }
}

void ATE::Close() {
  // TODO
}

void ATE::Snooze(const std::chrono::milliseconds& duration) { std::this_thread::sleep_for(duration); }
