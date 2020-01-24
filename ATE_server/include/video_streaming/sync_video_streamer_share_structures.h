#ifndef ATE_SERVER_VIDEO_STREAMING_SYNC_VIDEO_STREAMER_SHARE_STRUCTURES_H_
#define ATE_SERVER_VIDEO_STREAMING_SYNC_VIDEO_STREAMER_SHARE_STRUCTURES_H_

#include <semaphore.h>

#include <atomic>

namespace streamer {
namespace sync_video {

constexpr auto kV4L2BuffersNum = 16;
constexpr auto kSharedMemoryName = "/syncvideoshared";
constexpr auto kSharedMemoryBufferName = "/syncvideobuff";
constexpr auto kSemaphoreVHATRequest = "/sema_vhat_framereq";
constexpr auto kSyncVideoQSize = kV4L2BuffersNum;

struct SyncBuffer {
  unsigned char* start;
  unsigned int size;
  unsigned int alig_size;
  unsigned int accessible;
};

struct SyncStream {
  unsigned long size;
  int buf_number;
  unsigned int width;
  unsigned int height;
  unsigned int fps;
  unsigned int stride;
  unsigned int vstride;
  int vhat_own;
  SyncBuffer buf[kV4L2BuffersNum];
  int capture_queue[kSyncVideoQSize];
  int capture_q_in;
  int output_q_out;
  int sp_capture_queue[kSyncVideoQSize];
  int sp_capture_q_in;
  int sp_output_q_out;
  int output_queue[kSyncVideoQSize];
  int capture_q_out;
  int output_q_in;
  int sp_output_queue[kSyncVideoQSize];
  int sp_capture_q_out;
  int sp_output_q_in;
  std::atomic<int> vhat_frame_req;
};

struct SyncVideoContext {
  int shm_fd;
  int shmbuf_fd;
  sync_video::SyncStream* sync_video_stream;
  unsigned char* shmbuf;
  sem_t* lock_frame_request;
};

}  // namespace sync_video
}  // namespace streamer

#endif  // ATE_SERVER_VIDEO_STREAMING_SYNC_VIDEO_STREAMER_SHARE_STRUCTURES_H_
