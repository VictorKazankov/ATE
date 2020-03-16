#include "squish/wildcard.h"

#include "api.h"

squish::Wildcard::Wildcard(const std::string& name, const std::string& sync_version, const std::string& build_version,
                           const std::string& parent_screen, common::squish::CollectionMode mode) {
  this->name = name;
  this->sync_version = sync_version;
  this->build_version = build_version;
  this->parent_screen = parent_screen;
  this->mode = mode;
}

std::vector<squish::Object> squish::Wildcard::GetMatchObjects() {
  match_objects_ = API::GetObjectsDataByPattern(*this);
  return match_objects_;
}
