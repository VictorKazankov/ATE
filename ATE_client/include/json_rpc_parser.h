#ifndef JSON_RPC_PARSER_H_
#define JSON_RPC_PARSER_H_

#include "squish/squish_types.h"

#include <json/value.h>

namespace interaction {

/**
 * @brief JsonRpcParser provides functions to parse JSON RPC messages with responses regarding to APIs
 **/
class JsonRpcParser {
 public:
  /**
   * @brief CheckAndRaiseExceptionInCaseErrors performs validation for responses, throws an exception in case of any
   * problem with the response. Useful for APIs which not returns any value to Python
   * @param rpc response string for checking
   **/
  static void CheckAndRaiseExceptionInCaseErrors(const std::string& rpc);

  /**
   * @brief ParseWaitForObject performs parsing and validation for WaitForObject response, throws an exception in
   * case of any problem with the response
   * @param rpc response string for checking
   **/
  static squish::Object ParseWaitForObject(const std::string& rpc);

  /**
   * @brief ParseGetScreenshot performs parsing and validation for GetScreenshot response, throws an exception in
   * case of any problem with the response
   * @param rpc response string for checking
   **/
  static bool ParseGetScreenshot(const std::string& rpc);

  /**
   * @brief The function parses GetText api
   * @param rpc response string for checking
   * @return recognized text on sucess, in other case throws an exception InvalidRectangleCoordinates if coordinates are
   * invalid
   */
  static std::string ParseGetText(const std::string& rpc);

  /**
   * @brief The function parses GetObjectsDataByPattern and forming a list with squish objects
   * @param rpc Response string for checking
   * @return List of squish objects
   */
  static std::vector<squish::Object> ParseGetObjectsDataByPattern(const std::string& rpc);

 private:
  /**
   * @brief RpcStringToJsonStruct performs transformation and validation for responses as RPC string, throws an
   * exception in case of any problem with the response
   * @param rpc response string for transformation
   **/
  static Json::Value RpcStringToJsonStruct(const std::string& rpc);
};

}  // namespace interaction

#endif  // JSON_RPC_PARSER_H_
