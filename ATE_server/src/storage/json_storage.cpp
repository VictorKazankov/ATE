#include "storage/json_storage.h"

#include <fstream>
#include <stdexcept>

#include <jsoncpp/json/json.h>

#include "logger/logger.h"

namespace {
constexpr auto kIconName = "name";
constexpr auto kIconLocation = "location";
constexpr auto kJsonExtension = "json";
}  // namespace

storage::JsonStorage::JsonStorage(const fs::path& storage_path, const std::string& collection_name)
    : storage_path_(storage_path), collection_name_(collection_name) {}

bool storage::JsonStorage::Connect() { return LoadCollection(); }

fs::path storage::JsonStorage::ItemPath(const std::string& icon_name) const {
  try {
    return collection_.at(icon_name);
  } catch (const std::out_of_range& not_found) {
    logger::debug("[storage] Not found icon with name: {} ({})", icon_name, not_found.what());
    return {};
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
  collection_.clear();

  const fs::path collection_json_file = (storage_path_ / collection_name_).replace_extension(kJsonExtension);
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

  try {
    for (const auto& it : jsonobj) {
      collection_.emplace(it[kIconName].asString(), storage_path_ / it[kIconLocation].asCString());
    }
  } catch (const Json::LogicError& err) {
    // Strong exception warranty require not modified object if an exception occurs
    collection_.clear();
    logger::error("[storage] Failed parsing of {} ({})", collection_json_file, err.what());
    return false;
  } catch (...) {
    collection_.clear();
    logger::error("[storage] Unexpected error");
    throw;
  }

  return true;
}
