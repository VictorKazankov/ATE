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

  // Sync configuration changed error
  kInvalidSyncBuildVersion = -32003,
  kInvalidSyncVersion = -32002,
  kObjectNotFound = -32000,
};

}  // namespace rpc

#endif  // ATE_COMMON_RPC_ERROR_H_
