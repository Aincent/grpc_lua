// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../core/client/client_sync_reader.h"

#include <cassert>  // for assert()
#include <cstdint>  // for int64_t
#include <string>

#include "../../core/client/channel.h"          // for MakeSharedCall()
#include "../../core/client/impl/client_sync_reader_data.h"         // for Data
#include "../../core/client/impl/client_sync_reader_helper.h"       // for ClientSyncReaderHelper
#include "../../core/client/impl/cqtag/client_reader_init_cqtag.h"  // for ClientReaderInitCqTag
#include "../../core/common/status.h"           // for Status

namespace core {

ClientSyncReader::ClientSyncReader(const ChannelSptr& channel,
                                   const std::string& method,
                                   const std::string& request,
                                   int64_t timeout_ms)
    : data_sptr_(new Data) {
  assert(channel);
  CQueueForPluckSptr cq4p_sptr(new CQueueForPluck);
  CallSptr call_sptr = channel->MakeSharedCall(method, *cq4p_sptr, timeout_ms);
  data_sptr_->cq4p_sptr = cq4p_sptr;
  data_sptr_->call_sptr = call_sptr;

  ClientReaderInitCqTag tag(call_sptr);
  if (tag.Start(request)) {
    cq4p_sptr->Pluck(&tag);
    return;
  }

  data_sptr_->status.SetInternalError(
      "Failed to start client sync reader.");
}  // ClientSyncReader()

// Return false if error or end of stream.
bool ClientSyncReader::ReadOne(std::string* response) const {
  assert(response);
  Data& d = *data_sptr_;
  return ClientSyncReaderHelper::SyncReadOne(d.call_sptr, d.cq4p_sptr,
                                             *response, d.status);
}

Status ClientSyncReader::RecvStatus() const {
  const Data& d = *data_sptr_;
  if (!d.status.ok()) return d.status;
  return ClientSyncReaderHelper::SyncRecvStatus(d.call_sptr, d.cq4p_sptr);
}

// Set error status to break reading. Such as when parsing message failed.
void ClientSyncReader::SetErrorStatus(const Status& error_status) {
  assert(!error_status.ok());
  data_sptr_->status = error_status;
}

}  // namespace core
