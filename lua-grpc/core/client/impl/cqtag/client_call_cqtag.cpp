// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../../../core/client/impl/cqtag/client_call_cqtag.h"

namespace core {

Status ClientCallCqTag::GetResponse(std::string& response) const {
    // Todo: Get trailing metadata.
    if (!cod_client_recv_status_.IsStatusOk())
      return cod_client_recv_status_.GetStatus();
    return cod_recv_msg_.GetResultMsg(response);
}

}  // namespace core
