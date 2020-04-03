#include "db_manager_factory.h"

#include <db_manager/factory.h>

#include "logger/logger.h"

namespace adapter {

db_manager::AccessCredentials GetDefaultAccessCredentials() {
  const std::string reader_login = "reader";
  const std::string reader_password = "JustCuriousWhatIsInside";
  const std::string database = "ate";
  const std::string table = "icon_storage";
  return db_manager::AccessCredentials{reader_login, reader_password, database, table};
}

std::unique_ptr<db_manager::IconDataMapper> CreateDBManager(db_manager::AccessCredentials access_credentials) try {
  return db_manager::CreateIconDataMapper(std::move(access_credentials));
} catch (const db_manager::DbManagerError& err) {
  logger::error("[DBManagerAdapter] Can't create Icon Data Mapper: {}.", err.what());
  return {};
}

}  // namespace adapter
