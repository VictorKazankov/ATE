#include "ate.h"

#include "common.h"
#include "exceptions.h"
#include "interaction/VDP/vdp_interaction.h"
#include "logger/logger.h"
#include "storage/json_storage.h"
#include "video_streaming/stream_reader.h"

namespace {
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
    : storage_{
          std::make_unique<storage::JsonStorage>(common::Config().GetString(kDBSection, kPathOption, ""),
                                                 common::Config().GetString(kDBSection, kCollectionModeOption, ""))} {
  if (!storage_->Connect()) throw storage::ConnectionFailure{};

  interaction_ = std::make_unique<interaction::VDPInteraction>(
      io_context, common::Config().GetString(kBoardSection, kAddressOption, ""),
      common::Config().GetString(kBoardSection, kPortOption, ""),
      StrToDisplayType(common::Config().GetString(kBoardSection, kDisplayTypeOption, "")));

  streamer_ =
      std::make_unique<streamer::StreamReader>(common::Config().GetString(kBoardSection, kVideoStreamOption, ""));
}

ATE::~ATE() { Close(); }

void ATE::Close() {
  // TODO
}
