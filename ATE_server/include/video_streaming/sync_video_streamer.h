#ifndef ATE_SERVER_VIDEO_STREAMING_SYNC_VIDEO_STREAMER_H_
#define ATE_SERVER_VIDEO_STREAMING_SYNC_VIDEO_STREAMER_H_

#include "video_streaming/streamer.h"
#include "video_streaming/sync_video_streamer_share_structures.h"

namespace streamer {

/**
 * @brief SyncVideoStreamer is using for work with SyncVideo source
 **/
class SyncVideoStreamer : public Streamer {
 public:
  explicit SyncVideoStreamer(const cv::Size& frame_resolution);
  ~SyncVideoStreamer() override;

  /**
   * @brief The method gets video frame
   * @param reference to matrix (simple opencv matrix cv::Mat), made for increase performance.
   * @return 'true' in case of the frame has been read successfully otherwise false.
   **/
  bool Frame(cv::Mat& frame) override;

  /**
   * @brief The method changes resolution of video stream
   * @param x width of the frame
   * @param y height of the ftame
   **/
  void ChangeResolution(int x, int y) override;

 private:
  using FrameBufferPtr = unsigned char*;

  /**
   * @brief The method prepares shared memory for SyncVideo streamer
   * @param map mapped area
   * @param size the length of the mapping
   * @param name specifies the shared memory object to be created or opened
   * @param desc share memory file descriptor
   * @param flag is a bit mask for shared memory
   * @param mode shared memory mode
   * @param prot desired memory protection of the mapping
   * @return true in case of success, otherwise - false
   **/
  bool PrepareSharedMemory(void** map, size_t size, const char* name, int* desc, int flag, mode_t mode, int prot);

  /**
   * @brief Initializes sync video source
   * @return true in case of success, otherwise - false
   **/
  bool InitSyncVideo();

  /**
   * @brief Destroy sync video source and related structures
   **/
  void DestroySyncVideo();

  /**
   * @brief The function gets frame from sync video source
   * @param frame pointer to the frame
   * @return true in case of success, otherwise - false
   **/
  bool GetFrameFromSyncVideo(FrameBufferPtr* frame);

  sync_video::SyncVideoContext sync_video_context_;
  bool sync_video_inited_{false};
  cv::Size matrix_size_;
};

}  // namespace streamer

#endif  // ATE_SERVER_VIDEO_STREAMING_SYNC_VIDEO_STREAMER_H_
