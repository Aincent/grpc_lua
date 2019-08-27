// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../core/client/client_async_reader.h"

#include <cstdint>  // for int64_t
#include <string>

#include "../../core/client/channel_sptr.h"           // for ChannelSptr
#include "../../core/client/impl/client_async_reader_impl.h"  // for ClientAsyncReaderImpl
#include "../../core/client/msg_str_cb.h"             // for MsgStrCb
#include "../../core/client/status_cb.h"              // for StatusCb
#include "../common/completion_queue_sptr.h"  // for CompletionQueueSptr

namespace core {

ClientAsyncReader::ClientAsyncReader(const ChannelSptr& channel,
                                     const std::string& method,
                                     const std::string& request,
                                     const CompletionQueueSptr& cq_sptr,
                                     int64_t timeout_ms)
    : impl_sptr_(new ClientAsyncReaderImpl(channel, method, request, cq_sptr,
                                           timeout_ms)) {}

void ClientAsyncReader::ReadEach(
    const MsgStrCb& msg_cb, const StatusCb& status_cb /*= nullptr*/) const {
  impl_sptr_->Start(msg_cb, status_cb);
}

}  // namespace core
