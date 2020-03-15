#include "squish/wildcard.h"

#include "api.h"

squish::Wildcard::Wildcard(const std::string& name, const std::string& sync_version, const std::string& build_version,
                           const std::string& parent_name, common::squish::CollectionMode mode)
    : name_(name), sync_version_(sync_version), build_version_(build_version), parent_name_(parent_name), mode_(mode) {}

std::vector<squish::Object> squish::Wildcard::GetMatchObjects() {
  //match_objects_ = API::GetObjectsDataByPattern(*this);
  return match_objects_;
}
