#ifndef VDP_ATE_ATE_SERVER_SRC_ADAPTERS_DB_MANAGER_FACTORY_H_
#define VDP_ATE_ATE_SERVER_SRC_ADAPTERS_DB_MANAGER_FACTORY_H_

#include <memory>

#include <db_manager/access_credentials.h>
#include <db_manager/icon_data_mapper.h>

namespace adapter {

/**
 * @brief Get default database access information for ATE
 * @return Database access information
 */
db_manager::AccessCredentials GetDefaultAccessCredentials();

/**
 * @brief Create database manager
 * @param access_credentials Database access information
 * @return Instance of database manager in success, otherwise nullptr
 */
std::unique_ptr<db_manager::IconDataMapper> CreateDBManager(
    db_manager::AccessCredentials access_credentials = GetDefaultAccessCredentials());

}  // namespace adapter

#endif  // VDP_ATE_ATE_SERVER_SRC_ADAPTERS_DB_MANAGER_FACTORY_H_
