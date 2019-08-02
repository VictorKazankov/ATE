#include "storage/json_storage.h"

#include <fstream>
#include <iterator>
#include <stdexcept>
#include <vector>
#include <utility>

#include "json/json.h"

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

bool storage::JsonStorage::DoLoadCollection() {
  logger::info("[storage] Loading collection: {}", storage_path_ / collection_type_ / collection_build_ / collection_name_);

  const fs::path collection_json_file =
      (storage_path_ / collection_type_ / collection_build_ / collection_name_).replace_extension(kJsonExtension);
  std::ifstream file{collection_json_file};
  logger::trace("[storage] Parsing json file {}...", collection_json_file);

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
      std::string icon_name{it[kIconName].asString()};
      fs::path icon_path{storage_path_ / collection_type_ / collection_build_ / it[kIconLocation].asCString()};

      logger::trace("[storage] Adding icon \"{}\". Icon path: {}", icon_name, icon_path);
      new_collection.emplace_back(std::move(icon_name), std::move(icon_path));
    }
  } catch (const Json::LogicError& err) {
    logger::error("[storage] Failed to parse of {} at element: {} ({})", collection_json_file, new_collection.size(),
                  err.what());
    return false;
  }

  collection_.clear();
  collection_.reserve(new_collection.size());
  collection_.insert(std::make_move_iterator(new_collection.begin()), std::make_move_iterator(new_collection.end()));

  return true;
}

bool storage::JsonStorage::LoadCollection(const std::string& new_collection_type,
                                          const std::string& new_collection_build,
                                          const std::string& new_collection_name) {
  if (new_collection_type == collection_type_ && new_collection_build == collection_build_ &&
      new_collection_name == collection_name_) {
    logger::warn("[storage] Trying to load already-loaded collection: {}/{}", new_collection_type, new_collection_name);
    return false;
  }

  collection_type_ = new_collection_type;
  collection_build_ = new_collection_build;
  collection_name_ = new_collection_name;

  return DoLoadCollection();
}
