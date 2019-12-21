#ifndef VDP_ATE_ATE_SERVER_INCLUDE_FACTORY_RECOGNITION_FACTORY_H_
#define VDP_ATE_ATE_SERVER_INCLUDE_FACTORY_RECOGNITION_FACTORY_H_

#include <memory>
#include <string>

#include <recognition/detector.h>

namespace factory {

/**
 * @brief Create text detector depend on sync version
 * @param sync_version Sync version
 * @return Text detector instance
 */
std::unique_ptr<detector::Detector<std::string>> CreateTextDetector(const std::string& sync_version);

}  // namespace factory

#endif  // VDP_ATE_ATE_SERVER_INCLUDE_FACTORY_RECOGNITION_FACTORY_H_
