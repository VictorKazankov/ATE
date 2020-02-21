#ifndef ATE_API_H_
#define ATE_API_H_

#include "utils/squish_types.h"

#include "ate_interaction.h"
#include "squish/squish_types.h"

namespace API {

/**
 * @brief AteApi class represents custom APIs which can be processed by ATE.
 **/
class AteApi {
 public:
  /**
   * @brief This API allows changing sync version and sync build version without restart ate server.
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param sync_version Sync version
   * @param sync_build_version Sync build version
   * @throw InvalidSyncVersion In case of the sync_version is incorrect
   * @throw InvalidSyncBuildVersion In case of the sync_build_version is incorrect
   * @throw invalid_argument In case of the invalid arguments in the request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   */
  void ChangeSyncIconDB(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                        const uint64_t& correlation_id, const std::string& sync_version,
                        const std::string& sync_build_version) const;

  /**
   * @brief ChangeSyncMode provides changing collection mode API for active sync
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param collection_mode Collection mode
   * @throw InvalidSyncCollectionMode In case of the collection_mode is incorrect
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   */
  void ChangeSyncMode(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                      const uint64_t& correlation_id, common::squish::CollectionMode collection_mode) const;

  /**
   * @brief This API allows to take a screenshot of current screen and store it on the LVDS board
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param filename Name of the file (must contain .png extension for successfully saving)
   * @param location Location for saving of screenshot
   * @return True in case of screenshot saved
   * @throw VideoStreamNotFound In case of the video stream is not available
   * @throw EmptyScreenshotFileName In case of filename is empty"
   * @throw WrongScreenshotExtension In case of filename extension is not 'png'
   * @throw PermissionDenied In case of server does not have permission to make directory
   * @throw ImageAssemblingFailed In case of server can't save the screenshot (only in case of cv::imwrite failed)
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   */
  bool GetScreenshot(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                     const uint64_t& correlation_id, const std::string& filename, const std::string& location) const;

  /**
   * @brief GetText returns the text by specified coordinates
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param x1 X axis of the topleft coordinate
   * @param y1 Y axis of the topleft coordinate
   * @param x2 X axis of the bottom-right coordinate
   * @param y2 Y axis of the bottom-right coordinate
   * @throw VideoStreamNotFound In case of the video stream is not available
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   */
  std::string GetText(const std::shared_ptr<interaction::ATEInteraction>& ate_interaction,
                      const uint64_t& correlation_id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) const;

  /**
   * @brief Get object list by object name
   * @param object_name Name of object
   * @param correlation_id Correlation id for RPC
   * @return List of objects
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   */
  std::vector<squish::Object> GetObjectsDataByPattern(
      const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
      const std::string& object_name) const;

  /**
   * @brief Get object list by selecting a pattern
   * @param object_pattern Pattern for selection
   * @param correlation_id Correlation id for RPC
   * @return List of objects
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   */
  std::vector<squish::Object> GetObjectsDataByPattern(
      const std::shared_ptr<interaction::ATEInteraction>& ate_interaction, const uint64_t& correlation_id,
      const squish::Object& object_pattern) const;
};
}  // namespace API

#endif  // API_AGGREGATOR_H_
