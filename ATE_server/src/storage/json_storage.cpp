#include "storage/json_storage.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include <jsoncpp/json/json.h>

namespace {
constexpr auto kIconName = "name";
constexpr auto kIconLocation = "location";
}  // namespace

storage::JsonStorage::JsonStorage(const std::string& storage_path, const std::string& collection_name)
    : storage_path_(storage_path), collection_name_(collection_name) {}

bool storage::JsonStorage::Connect() { return LoadCollection(); }

std::string storage::JsonStorage::ItemPath(const std::string& icon_name) const {
  auto it = collection_.find(icon_name);
  if (it != collection_.end()) {
    return it->second;
  } else {
    // TODO(MShvaiko@luxoft.com) : Create custom exception class and use instead throwing string
    std::clog << "ERROR : Undefind icon \'" << icon_name << "\'" << std::endl;
    throw std::runtime_error("ERROR : Undefind icon \'" + icon_name + "\'");
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
    // TODO(MShvaiko@luxoft.com) : Create custom exception class and use instead throwing string
    std::clog << "ERROR : File \'" << path << "\' can't open." << std::endl;
    return false;
  }

  if (!reader.parse(file, jsonobj, false)) {
    std::cerr << "ERROR : Parsing of " << path << " failure." << std::endl;
    return false;
  }

  try {
    for (const auto& it : jsonobj) {
      collection_.emplace(it[kIconName].asString(), it[kIconLocation].asString());
    }
  } catch (const Json::LogicError& err) {
    std::cerr << "ERROR : " << err.what() << std::endl;
    return false;
  }

  return true;
}
