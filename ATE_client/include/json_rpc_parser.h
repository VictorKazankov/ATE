#ifndef JSON_RPC_PARSER_H_
#define JSON_RPC_PARSER_H_

#include "squish/squish_types.h"

#include <json/value.h>

namespace interaction {

namespace impl {

/**
 * @brief The function verifies is message with JSON valid
 * @return True in case of valid JSON, otherwise false
 **/
bool IsJsonStructureValid(const std::string& message, Json::Value& value);

/**
 * @brief The function handles errors related to JSON structure. In case of error, it throws the appropriate exception
 */
[[noreturn]] void ErrorHandler(const Json::Value& error);

}  // namespace impl

/**
 * @brief JsonRpcParser provides functions to parse JSON RPC messages with responses regarding to APIs
 **/
class JsonRpcParser {
 public:
  /**
   * @brief CheckAndRaiseExceptionInCaseErrors performs validation for responses, throws an exception in case of any
   *        problem with the response. Useful for APIs which not returns any value to Python
   * @param rpc Response string for checking
   **/
  static void CheckAndRaiseExceptionInCaseErrors(const std::string& rpc);

  /**
   * @brief ParseWaitForObject performs parsing and validation for WaitForObject response, throws an exception in
   *        case of any problem with the response
   * @param rpc Response string for checking
   * @return Constructed and valid squish::Object otherwise empty object
   **/
  static squish::Object ParseWaitForObject(const std::string& rpc);

  /**
   * @brief ParseGetScreenshot performs parsing and validation for GetScreenshot response, throws an exception in
   *        case of any problem with the response
   * @param rpc Response string for checking
   * @return Result of 'GetScreenshot' operation. True on success otherwise false
   **/
  static bool ParseGetScreenshot(const std::string& rpc);

  /**
   * @brief The function parses GetText api
   * @param rpc Response string for checking
   * @return Recognized text on sucess, in other case throws an exception InvalidRectangleCoordinates if coordinates are
   *         invalid
   */
  static std::string ParseGetText(const std::string& rpc);

  /**
   * @brief The function parses GetObjectsDataByPattern and forming a list with squish objects
   * @param rpc Response string for checking
   * @return List of squish objects
   */
  static std::vector<squish::Object> ParseGetObjectsDataByPattern(const std::string& rpc);

  /**
   * @brief The function parses GetImagesDiscrepancy and gets percent of discrepancy of two images
   * @param rpc Response string for checking
   * @return Percent of discrepancy two images
   * @throw runtime_error In case if missing mandatory field in response
   */
  static int ParseGetImagesDiscrepancy(const std::string& rpc);

  /**
   * @brief The function parses CaptureFrames and forming a list with frame names
   * @param rpc Response string for checking
   * @return List of frame names
   */
  static std::vector<std::string> ParseCaptureFrames(const std::string& rpc);

  /**
   * @brief The function parses FindAllImages and forming a list with found objects
   * @param rpc Response string for checking
   * @return List of found objects
   * @throw VideoStreamingError In case of the video stream is not available
   * @throw runtime_error In case of an internal error
   * @throw invalid_argument In case of the invalid arguments in the request
   * @throw InvalidRectangleCoordinates In case top-left and bottom-right coordinates are mixed up or
   *                                    produced rectangle has zero height/width or is out of frame boundaries
   */
  static std::vector<squish::Object> ParseFindAllImages(const std::string& rpc);

 private:
  /**
   * @brief RpcStringToJsonStruct performs transformation and validation for responses as RPC string, throws an
   *        exception in case of any problem with the response
   * @param rpc Response string for transformation
   **/
  static Json::Value RpcStringToJsonStruct(const std::string& rpc);
};

}  // namespace interaction

#endif  // JSON_RPC_PARSER_H_
