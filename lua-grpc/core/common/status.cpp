// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../core/common/status.h"

namespace core {

const Status& Status::OK = Status();
const Status& Status::CANCELLED = Status(GRPC_STATUS_CANCELLED, "Cancelled");
const Status& Status::UNIMPLEMENTED = Status(GRPC_STATUS_UNIMPLEMENTED, "Unimplemented");

Status Status::InternalError(const std::string& details) {
  return Status(GRPC_STATUS_INTERNAL, details);
}

void Status::SetInternalError(const std::string& error_msg) {
  core_ = GRPC_STATUS_INTERNAL;
  details_ = error_msg;
}

}  // namespace core
