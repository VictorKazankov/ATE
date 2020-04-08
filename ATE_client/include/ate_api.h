#ifndef ATE_API_H_
#define ATE_API_H_

#include <memory>
#include <vector>

#include "interaction.h"
#include "squish/squish_types.h"
#include "squish/wildcard.h"
#include "utils/squish_types.h"

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
  void ChangeSyncIconDB(const std::shared_ptr<interaction::Interaction>& ate_interaction,
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
  void ChangeSyncMode(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                      common::squish::CollectionMode collection_mode) const;

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
  bool GetScreenshot(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                     const std::string& filename, const std::string& location) const;

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
  std::string GetText(const std::shared_ptr<interaction::Interaction>& ate_interaction, const uint64_t& correlation_id,
                      uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) const;

  /**
   * @brief Get object list by selecting a pattern
   * @param object_pattern Pattern for selection
   * @param correlation_id Correlation id for RPC
   * @param wildcard Pattern for selection
   * @param object_name Name of Object
   * @return List of objects
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   */
  std::vector<squish::Object> GetObjectsDataByPattern(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                                                      const uint64_t& correlation_id,
                                                      const squish::Wildcard& wildcard) const;

  std::vector<squish::Object> GetObjectsDataByPattern(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                                                      const uint64_t& correlation_id,
                                                      const std::string& object_name) const;

  /**
   * @brief Gets the difference between the two images as a percentage
   * @param icon_path_second Path to the second image
   * @param icon_path_first Path to the first image
   * @param top_left_coordinate The top left coordinate of the comparison area
   * @param bottom_right_coordinate The bottom right coordinate of the comparison area
   * @return Percentage of discrepancy
   * @throw InvalidRectangleCoordinates In case top-left and bottom-right coordinates are mixed up or
   *                                    produced rectangle has zero height/width or is out of frame boundaries
   * @throw ComparingImageIncorrectSize In case incorrect size of image
   * @throw ComparingImageNotExist In case comparing images doesn't exist
   * @throw EmptyScreenshotFileName In case comparing image file path is empty
   * @throw PermissionDenied In case error read permissions
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   * @throw UnsupportedFileType In case of incorrect image file type
   */
  int GetImagesDiscrepancy(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                           const uint64_t& correlation_id, const std::string& icon_path_second,
                           const std::string& icon_path_first, const common::Point& top_left_coordinate,
                           const common::Point& bottom_right_coordinate) const;

  /**
   * @brief CaptureFrames saves frames in specified path with desired area and returns an array of stored frame names
   * @param ate_interaction Structure provides the ability to communicate with ATE
   * @param correlation_id Correlation id for RPC
   * @param interval Interval in miliseconds
   * @param duration Duration in mileseconds
   * @param top_left Top left point
   * @param bottom_right Bottom right point
   * @param path Path where frames will be stored
   * @return List of frame names
   * @throw InvalidDuration In case if the duration longer than 5s
   * @throw VideoStreamNotFound In case of the video stream is not available
   * @throw PermissionDenied In case of server does not have permission to make directory
   * @throw ImageAssemblingFailed In case of server can't save the screenshot (only in case of cv::imwrite failed)
   * @throw InternalError In case of file system errors, bad alloc
   * @throw NoAvailableDiskSpace In case if no available space on a disk
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   */
  std::vector<std::string> CaptureFrames(const std::shared_ptr<interaction::Interaction>& ate_interaction,
                                         const uint64_t& correlation_id, int interval, int duration,
                                         const common::Point& top_left, const common::Point& bottom_right,
                                         const std::string& path) const;
};
}  // namespace API

#endif  // API_AGGREGATOR_H_
