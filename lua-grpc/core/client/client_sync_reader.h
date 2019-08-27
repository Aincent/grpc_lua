// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)
#ifndef CORE_CLIENT_SYNC_READER_H
#define CORE_CLIENT_SYNC_READER_H

#include <cstdint>  // for int64_t
#include <string>

#include "../../core/client/channel_sptr.h"  // for ChannelSptr
#include "../../core/common/support/config.h"                // for GRPC_FINAL

namespace core {

class Status;
struct ClientSyncReaderData;

// Copyable. Client sync reader.
class ClientSyncReader GRPC_FINAL {
 public:
  ClientSyncReader(const ChannelSptr& channel, const std::string& method,
                   const std::string& request, int64_t timeout_ms);

 public:
  // Return false if error or end of stream.
  bool ReadOne(std::string* response) const;
  Status RecvStatus() const;

  // Set error status to break reading. Such as when parsing message failed.
  void SetErrorStatus(const Status& error_status);

 private:
  // Wrap all data in shared struct pointer to make copy quick.
  using Data = ClientSyncReaderData;
  using DataSptr = std::shared_ptr<Data>;
  DataSptr data_sptr_;
};  // class ClientSyncReader

}  // namespace core
#endif  // CORE_CLIENT_SYNC_READER_H
