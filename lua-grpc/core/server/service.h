// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)
#ifndef CORE_SERVER_SERVICE_H
#define CORE_SERVER_SERVICE_H

#include <string>

#include "../../core/common/call_sptr.h"                 // for CallSptr
#include "../common/support/core_api.h"  // for CORE_API

struct grpc_byte_buffer;

namespace core {

// Service base class.
class CORE_API Service {
 public:
  virtual const std::string& GetFullName() const = 0;
  virtual size_t GetMethodCount() const = 0;
  virtual bool IsMethodClientStreaming(size_t method_index) const = 0;
  virtual const std::string& GetMethodName(size_t method_index) const = 0;

  // TODO: need request_context. Need client address in Ctr?
  virtual void CallMethod(size_t method_index, grpc_byte_buffer* request,
                          const CallSptr& call_sptr) = 0;
};  // class Service

}  // namespace core
#endif  // CORE_SERVER_SERVICE_H
