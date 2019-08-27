// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../../../core/server/impl/cqtag/server_writer_write_cqtag.h"

#include "../../../../core/common/impl/call.h"             // for StartBatch()
#include "../../../common/impl/call_operations.h"  // for CallOperations

namespace core {

bool ServerWriterWriteCqTag::Start(
    const std::string& message, bool send_init_md) {
  CallOperations ops;
  if (send_init_md) {
    // Todo: set init_md
    ops.SendInitMd(cod_send_init_md_);
  }
  ops.SendMsg(message, cod_send_msg_);  // XXX check result
  return GetCallSptr()->StartBatch(ops, this);
}

}  // namespace core
