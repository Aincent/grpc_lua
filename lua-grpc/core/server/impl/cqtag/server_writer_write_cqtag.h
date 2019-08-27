// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_SERVER_SERVER_WRITER_WRITE_CQTAG_H
#define CORE_SERVER_SERVER_WRITER_WRITE_CQTAG_H

#include <grpc/support/port_platform.h>       // for GRPC_MUST_USE_RESULT

#include "../../../../core/common/impl/cqtag/general_call_cqtag.h"  // for GeneralCallCqTag
#include "../../../common/support/config.h"				   // for GRPC_FINAL
#include "../../../common/impl/call_op_data.h"              // for CodSendMsg

namespace core {

class ServerWriterWriteCqTag GRPC_FINAL : public GeneralCallCqTag {
 public:
  explicit ServerWriterWriteCqTag(const CallSptr& call_sptr)
      : GeneralCallCqTag(call_sptr) {
    assert(call_sptr);
  }

  bool Start(const std::string& message, bool send_init_md)
      GRPC_MUST_USE_RESULT;

 private:
  CodSendInitMd cod_send_init_md_;
  CodSendMsg cod_send_msg_;
};  // class ServerWriterWriteCqTag

}  // namespace core

#endif  // CORE_SERVER_SERVER_WRITER_WRITE_CQTAG_H
