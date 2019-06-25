#include "storage/json_storage.h"

#include <fstream>
#include <iterator>
#include <stdexcept>
#include <vector>

#include <jsoncpp/json/json.h>

#include "logger/logger.h"

namespace {
constexpr auto kIconName = "name";
constexpr auto kIconLocation = "location";
constexpr auto kJsonExtension = "json";
}  // namespace

storage::JsonStorage::JsonStorage(const fs::path& storage_path) : storage_path_(storage_path) {}

fs::path storage::JsonStorage::ItemPath(const std::string& icon_name) const {
  try {
    return collection_.at(icon_name);
  } catch (const std::out_of_range& not_found) {
    logger::debug("[storage] Not found icon with name: {} ({})", icon_name, not_found.what());
    return {};
  }
}

bool storage::JsonStorage::LoadCollection(const std::string& new_collection_type,
                                          const std::string& new_collection_name) {
  if (new_collection_type == collection_type_ && new_collection_name == collection_name_) {
    logger::warn("[storage] Trying to load already-loaded collection: {}/{}", new_collection_type, new_collection_name);
    return false;
  }

  const fs::path collection_json_file =
      (storage_path_ / new_collection_type / new_collection_name).replace_extension(kJsonExtension);
  std::ifstream file{collection_json_file};

  if (!file) {
    logger::error("[storage] File {} can't be open.", collection_json_file);
    return false;
  }

  Json::Reader reader;
  Json::Value jsonobj;

  if (!reader.parse(file, jsonobj, false)) {
    logger::error("[storage] Parsing of {} failure: {}", collection_json_file, reader.getFormattedErrorMessages());
    return false;
  }

  std::vector<std::pair<std::string, fs::path>> new_collection;

  try {
    new_collection.reserve(jsonobj.size());
    for (const auto& it : jsonobj) {
      new_collection.emplace_back(it[kIconName].asString(),
                                  storage_path_ / new_collection_type / it[kIconLocation].asCString());
    }
  } catch (const Json::LogicError& err) {
    logger::error("[storage] Failed to parse of {} at element: {} ({})", collection_json_file, new_collection.size(),
                  err.what());
    return false;
  }

  collection_.clear();
  collection_.reserve(new_collection.size());
  collection_.insert(std::make_move_iterator(new_collection.begin()), std::make_move_iterator(new_collection.end()));

  collection_type_ = new_collection_type;
  collection_name_ = new_collection_name;

  return true;
}
