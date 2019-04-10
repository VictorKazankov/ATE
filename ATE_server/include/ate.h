#ifndef ATE_SERVER_ATE_H_
#define ATE_SERVER_ATE_H_

#include <memory>
#include "interaction/interaction.h"
#include "reporting/reporter.h"
#include "storage/storage.h"
#include "video_streaming/matching/detector.h"
#include "video_streaming/streamer.h"

/**
 * Automated Test Environment class for communication with LVDS board
 **/
class ATE {
 private:
  std::unique_ptr<storage::Storage> storage_;
  std::unique_ptr<interaction::Interaction> interaction_;
  std::unique_ptr<streamer::Streamer> streamer_;
  std::unique_ptr<detector::Detector> detector_;
  std::unique_ptr<reporter::Reporter> reporter_;

 public:
  ATE();
  ~ATE();

  /**
   * Closes connections
   **/
  void Close();
};

#endif  // ATE_SERVER_ATE_H_
