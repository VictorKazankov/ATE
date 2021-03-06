#ifndef ATE_COMMON_RPC_ERROR_H_
#define ATE_COMMON_RPC_ERROR_H_

namespace rpc {

enum class Error {
  kParseError = -32700,
  kInvalidRequest = -32600,
  kMethodNotFound = -32601,
  kInvalidParams = -32602,
  kInternalError = -32603,

  // Values from -32000 to -32099 reserved for implementation-defined server-errors

  // Screenshot error
  kNoAvailableDiskSpace = -32015,
  // Image Discrepancy error
  kUnsupportedFileType = -32014,
  kComparingImageIncorrectSize = -32013,
  kComparingImageNotExist = -32012,
  // GetText error
  kInvalidRectangleCoordinates = -32011,
  // Screenshot error
  kImageAssemblingFailed = -32010,
  kWrongExtension = -32009,
  kPermissionDenied = -32008,
  kEmptyFileName = -32007,
  // Long press error
  kInvalidDuration = -32006,
  // Sync configuration changed error
  kInvalidSyncCollectionMode = -32004,
  kInvalidSyncBuildVersion = -32003,
  kInvalidSyncVersion = -32002,
  // Object recognition error
  kVideoStreamNotFound = -32001,
  kObjectNotFound = -32000,
};

}  // namespace rpc

#endif  // ATE_COMMON_RPC_ERROR_H_
