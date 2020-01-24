#include "video_streaming/sync_video_streamer.h"

#include <sys/mman.h>
#include <cerrno>

#include <opencv2/imgproc.hpp>

#include "logger/logger.h"

using namespace streamer;

namespace {
constexpr auto kOwnerReadAndWrite = 0600;

inline unsigned int ByteToPixel(unsigned int bytes) { return bytes / 4; }
}

void PrintErrorMsgWithErrno(const std::string& msg) {
  std::string error_msg = msg;
  error_msg.append(". ");
  error_msg.append(std::strerror(errno));
  logger::error(error_msg);
}

SyncVideoStreamer::SyncVideoStreamer(const cv::Size& frame_resolution) : matrix_size_(frame_resolution) {
  if (!InitSyncVideo()) {
    logger::error("[SyncVideoStreamer] Initialization has failed");
  }
}

SyncVideoStreamer::~SyncVideoStreamer() { DestroySyncVideo(); }

bool SyncVideoStreamer::Frame(cv::Mat& frame) {
  FrameBufferPtr frame_ptr{nullptr};

  if (!GetFrameFromSyncVideo(&frame_ptr)) {
    logger::error("[SyncVideoStreamer] Getting frame from SyncVideo has failed");
    return false;
  }

  try {
    unsigned int stride = ByteToPixel(sync_video_shared_memory_.sync_video_stream->stride);
    
    logger::debug("[SyncVideoStreamer] Frame resolution [{},{}]; Real image [{},{}]", matrix_size_.width,
                  matrix_size_.height, stride,
                  sync_video_shared_memory_.sync_video_stream->height);

    const cv::Mat buffer_mat{static_cast<int>(sync_video_shared_memory_.sync_video_stream->height),
                             static_cast<int>(stride),
                             CV_8UC4, frame_ptr};
    cv::cvtColor(buffer_mat, frame, cv::COLOR_BGR2RGB);
  } catch (...) {
    logger::error("[SyncVideoStreamer] An unexpected error occurs while extracting data from the buffer");
    return false;
  }

  return true;
}

void SyncVideoStreamer::ChangeResolution(int x, int y) {
  logger::info("[SyncVideoStreamer] Changing screen resolution to [{};{}]", x, y);

  matrix_size_ = cv::Size{x, y};
  DestroySyncVideo();

  if (!InitSyncVideo()) {
    logger::error("[SyncVideoStreamer] Initialization has failed");
  }
}

bool SyncVideoStreamer::InitSyncVideo() {
  bool result;

  result = PrepareSharedMemory(reinterpret_cast<void**>(&sync_video_shared_memory_.sync_video_stream),
                               sizeof(sync_video::SyncStream), sync_video::kSharedMemoryName,
                               &sync_video_shared_memory_.shm_fd, O_RDWR, kOwnerReadAndWrite, PROT_READ | PROT_WRITE);
  if (!result) {
    logger::error("[SyncVideoStreamer] Sync stream's shared memory initialization has failed");
    return result;
  }
  logger::info("[SyncVideoStreamer] Sync stream's shared memory initialization was successful");

  result = PrepareSharedMemory(reinterpret_cast<void**>(&sync_video_shared_memory_.shmbuf),
                               sync_video_shared_memory_.sync_video_stream->size, sync_video::kSharedMemoryBufferName,
                               &sync_video_shared_memory_.shmbuf_fd, O_RDWR, kOwnerReadAndWrite, PROT_READ);
  if (!result) {
    logger::error("[SyncVideoStreamer] Buffer's shared memory initialization has failed");
    return result;
  }
  logger::info("[SyncVideoStreamer] Buffer's shared memory initialization was successful");

  if ((sync_video_shared_memory_.lock_frame_request = sem_open(sync_video::kSemaphoreVHATRequest, 0)) == SEM_FAILED) {
    logger::error("[SyncVideoStreamer] Shared memory semaphore initialization has failed");
    return false;
  }
  logger::info("[SyncVideoStreamer] Shared memory semaphore initialization was successful");

  sync_video_shared_memory_.sync_video_stream->vhat_frame_req.store(0);

  logger::info("[SyncVideoStreamer] Initialization was successful");
  sync_video_inited_ = true;
  return true;
}

void SyncVideoStreamer::DestroySyncVideo() {
  sync_video_inited_ = false;

  if (sem_close(sync_video_shared_memory_.lock_frame_request)) {
    PrintErrorMsgWithErrno("Semaphore closing error");
  }

  if (munmap(sync_video_shared_memory_.shmbuf, sync_video_shared_memory_.sync_video_stream->size)) {
    PrintErrorMsgWithErrno("Buffer's shared memory closing error");
  }
  if (close(sync_video_shared_memory_.shmbuf_fd)) {
    PrintErrorMsgWithErrno("Buffer's descriptor closing error");
  }

  if (munmap(sync_video_shared_memory_.sync_video_stream, sizeof(sync_video::SyncStream))) {
    PrintErrorMsgWithErrno("Sync stream's shared memory closing error");
  }
  if (close(sync_video_shared_memory_.shm_fd)) {
    PrintErrorMsgWithErrno("Sync stream descriptor closing error");
  }
}

bool SyncVideoStreamer::PrepareSharedMemory(void** map, size_t size, const char* name, int* desc, int flag, mode_t mode,
                                            int prot) {
  int fd = shm_open(name, flag, mode);
  if (fd < 0) {
    *desc = -1;
    PrintErrorMsgWithErrno("[SyncVideoStreamer] shm_open has failed");
    return false;
  }

  *desc = fd;
  if (ftruncate(fd, size) != 0) {
    PrintErrorMsgWithErrno("[SyncVideoStreamer] ftruncate has failed");
    return false;
  }

  *map = mmap(nullptr, size, prot, MAP_SHARED, fd, 0);
  if (MAP_FAILED == *map) {
    PrintErrorMsgWithErrno("[SyncVideoStreamer] mmap has failed");
    return false;
  }

  return true;
}

bool SyncVideoStreamer::GetFrameFromSyncVideo(FrameBufferPtr* frame) {
  if (!sync_video_inited_) {
    logger::warn("[SyncVideoStreamer] Video source wasn't initialized");

    if (!InitSyncVideo()) {
      logger::error("[SyncVideoStreamer] Initialization has failed");
      return false;
    }
  }

  sync_video::SyncStream* sync_video_stream = sync_video_shared_memory_.sync_video_stream;
  sync_video_stream->vhat_frame_req.store(1);
  sem_wait(sync_video_shared_memory_.lock_frame_request);
  int idx = sync_video_stream->vhat_own;

  if (idx < 0 || idx >= sync_video_stream->buf_number) {
    logger::error("[SyncVideoStreamer] Wrong shmem buffer index {}", idx);
    return false;
  }

  *frame = sync_video_shared_memory_.shmbuf + sync_video_stream->buf[idx].alig_size * idx;
  return true;
}
