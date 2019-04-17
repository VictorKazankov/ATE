#include "storage/json_storage.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include <jsoncpp/json/json.h>

namespace {
constexpr auto kIconName = "name";
constexpr auto kIconLocation = "location";
}  // namespace

storage::JsonStorage::JsonStorage(const std::string& storage_name, const std::string& collection_name)
    : storage_name_(storage_name), collection_name_(collection_name) {
  LoadCollection();
}

void storage::JsonStorage::Connect(const std::string& storage_name) {
  if (storage_name != storage_name_) {
    storage_name_ = storage_name;
    LoadCollection();
  } else {
    // TODO(MShvaiko@luxoft.com) : Change 'clog' to logger system
    std::clog << "INFO : Storage and collection have already upload." << std::endl;
  }
}

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

storage::JsonStorage::IconMap storage::JsonStorage::Collection() const { return collection_; }

void storage::JsonStorage::ChangeCollection(const std::string& collection_name) {
  if (collection_name_ != collection_name) {
    collection_name_ = collection_name;
    LoadCollection();
  } else {
    // TODO(MShvaiko@luxoft.com) : Change 'clog' to logger system
    std::clog << "INFO : Storage and collection have already upload." << std::endl;
  }
}

void storage::JsonStorage::LoadCollection() {
  const auto path = storage_name_ + "/" + collection_name_;
  std::ifstream file(path);
  Json::Reader reader;
  Json::Value jsonobj;

  if (!collection_.empty()) {
    collection_.clear();
  }

  if (!file) {
    // TODO(MShvaiko@luxoft.com) : Create custom exception class and use instead throwing string
    std::clog << "ERROR : File \'" << path << "\' can't open." << std::endl;
    throw std::runtime_error("ERROR : File \'" + path + "\' can't open.");
  }

  if (reader.parse(file, jsonobj, false)) {
    try {
      for (const auto& it : jsonobj) {
        collection_.emplace(it[kIconName].asString(), it[kIconLocation].asString());
      }
    } catch (const Json::LogicError& err) {
      std::cerr << "ERROR : " << err.what() << std::endl;
      throw;
    }
  } else {
    std::cerr << "ERROR : Parsing of " << path << " failure." << std::endl;
    throw std::runtime_error("ERROR : \'" + reader.getFormattedErrorMessages() + "\'.");
  }
}
