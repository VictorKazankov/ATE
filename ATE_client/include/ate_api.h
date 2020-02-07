#ifndef ATE_API_H_
#define ATE_API_H_

#include "utils/squish_types.h"

#include "ate_interaction.h"

namespace API {

/**
 * @brief AteApi class represents custom APIs which can be processed by ATE.
 **/
class AteApi {
 public:
  /**
   * @brief This API allows changing sync version and sync build version without restart ate server.
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param sync_version Sync version
   * @param sync_build_version Sync build version
   */
  void ChangeSyncIconDB(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                        const uint64_t& correlation_id, const std::string& sync_version,
                        const std::string& sync_build_version) const;

  /**
   * @brief ChangeSyncMode provides changing collection mode API for active sync
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param collection_mode Collection mode
   */
  void ChangeSyncMode(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                      const uint64_t& correlation_id, common::squish::CollectionMode collection_mode) const;

  /**
   * @brief This API allows to take a screenshot of current screen and store it on the LVDS board
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param filename - name of the file (must contain .png extension for successfully saving)
   * @param location - location for saving of screenshot
   * @return true in case of screenshot saved
   * @throw VideoStreamNotFound - in case of the video stream is not available
   * @throw EmptyScreenshotFileName - in case of filename is empty"
   * @throw WrongScreenshotExtension - in case of filename extansion is not 'png'
   * @throw PermissionDenied - in case of server does not have permission to make directory
   * @throw ImageAssemblingFailed - in case of server can't save the screenshot (only in case of cv::imwrite failed)
   * @throw InternalError - in case of file system errors, bad alloc
   */
  bool GetScreenshot(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                     const uint64_t& correlation_id, const std::string& filename, const std::string& location) const;

  /**
   * @brief GetText returns the text by specified coordinates
   * @param ate_interaction structure provides the ability to communicate with ATE
   * @param correlation_id - correlation id for RPC
   * @param x1 x axis of the topleft coordinate
   * @param y1 y axis of the topleft coordinate
   * @param x2 x axis of the bottom-right coordinate
   * @param y2 y axis of the bottom-right coordinate
   */
  std::string GetText(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                      const uint64_t& correlation_id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) const;
};
}  // namespace API

#endif  // API_AGGREGATOR_H_
