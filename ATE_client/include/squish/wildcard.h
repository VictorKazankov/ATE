#ifndef SQUISH_WILDCARD_H_
#define SQUISH_WILDCARD_H_

#include <string>
#include <vector>

#include "utils/object_data_type.h"
#include "utils/squish_types.h"

#include "squish_types.h"

namespace squish {

/**
 * @name Wildcard
 * @brief Wildcard provides the ability to extract information about objects from a database by pattern and stores this
 *        information inside 'match_objects'.
 */
class Wildcard : public common::ObjectDataIdentity {
 public:
  /**
   * @brief The constructor of Wildcard takes patterns 'name', 'sync_version', 'build_version', 'parent_name', 'mode'
   *        for the further searching of information in the database and store it inside 'match_objects.
   * @param name The name of the object for searching objects in the database
   * @param sync_version The sync version for searching objects in the database
   * @param build_version The build version for searching objects in the database
   * @param parent_screen The parent screen name for searching objects in the database
   * @param mode The mode for searching objects in the database
   * @throw NoConnectionEstablished In case of no connection was established to server-side
   * @throw invalid_argument In case of the invalid arguments in request
   * @throw runtime_error In case of an internal error, parse error, invalid request, a method not found
   */
  explicit Wildcard(const std::string& name, const std::string& sync_version = "", const std::string& build_version = "",
           const std::string& parent_screen = "",
           common::squish::CollectionMode mode = common::squish::CollectionMode::kNone);

  /**
   * @name GetMatchObjects
   * @brief This function returns results that matches to defined criterias
   * @return The non-empty vector of the 'squish objects' if pattern matches to any object in the database, otherwise
   *         empty vector
   */
  std::vector<Object> GetMatchObjects();

 private:
  std::vector<Object> match_objects_;
};

}  // namespace squish

#endif  // SQUISH_WILDCARD_H_
