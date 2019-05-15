#include "ate.h"
#include "interaction/VDP/vdp_interaction.h"
#include "storage/json_storage.h"
#include "utils/logger.h"
#include "video_streaming/stream_reader.h"

namespace {
const std::string kConfigFile = VHAT_DATA_PATH "/config.ini";
const std::string kDBSection = "DB";
const std::string kPathOption = "Path";
const std::string kCollectionModeOption = "CollectionMode";
const std::string kBoardSection = "BOARD";
const std::string kAddressOption = "Address";
const std::string kPortOption = "Port";
const std::string kDisplayTypeOption = "DisplayType";
const std::string kVideoStreamOption = "VideoStream";

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
    : config_{kConfigFile},
      logger_{config_},
      storage_{std::make_unique<storage::JsonStorage>(config_.GetString(kDBSection, kPathOption, ""),
                                                      config_.GetString(kDBSection, kCollectionModeOption, ""))} {
  if (!storage_->Connect()) {
    logger::critical("[ate] Failed to connect to storage.");
    exit(EXIT_FAILURE);
  }

  interaction_ = std::make_unique<interaction::VDPInteraction>(
      io_context, config_.GetString(kBoardSection, kAddressOption, ""),
      config_.GetString(kBoardSection, kPortOption, ""),
      StrToDisplayType(config_.GetString(kBoardSection, kDisplayTypeOption, "")));

  streamer_ = std::make_unique<streamer::StreamReader>(config_.GetString(kBoardSection, kVideoStreamOption, ""));
}

ATE::~ATE() { Close(); }

void ATE::Close() {
  // TODO
}
