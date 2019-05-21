#include "storage/json_storage.h"

#include <fstream>
#include <stdexcept>

#include <jsoncpp/json/json.h>

#include "logger/logger.h"

namespace {
constexpr auto kIconName = "name";
constexpr auto kIconLocation = "location";
}  // namespace

storage::JsonStorage::JsonStorage(const std::string& storage_path, const std::string& collection_name)
    : storage_path_(storage_path), collection_name_(collection_name) {}

bool storage::JsonStorage::Connect() { return LoadCollection(); }

std::string storage::JsonStorage::ItemPath(const std::string& icon_name) const {
  try {
    return collection_.at(icon_name);
  } catch (const std::out_of_range& not_found) {
    logger::debug("[storage] Not found icon with name: {} ({})", icon_name, not_found.what());
    return std::string{};
  }
}

bool storage::JsonStorage::ChangeCollection(const std::string& collection_name) {
  if (collection_name == collection_name_) {
    return false;
  }

  collection_name_ = collection_name;
  return LoadCollection();
}

bool storage::JsonStorage::LoadCollection() {
  const auto path = storage_path_ + "/" + collection_name_;
  std::ifstream file(path);
  Json::Reader reader;
  Json::Value jsonobj;

  if (!collection_.empty()) {
    collection_.clear();
  }

  if (!file) {
    logger::error("[storage] File \'{}\' can't be open.", path);
    return false;
  }

  if (!reader.parse(file, jsonobj, false)) {
    logger::error("[storage] Parsing of \'{}\' failure.", path);
    return false;
  }

  try {
    for (const auto& it : jsonobj) {
      collection_.emplace(it[kIconName].asString(), it[kIconLocation].asString());
    }
  } catch (const Json::LogicError& err) {
    logger::error("[storage] {}", err.what());
    // Strong exception warranty require not modified object if an exception occurs
    collection_.clear();
    return false;
  }

  return true;
}
