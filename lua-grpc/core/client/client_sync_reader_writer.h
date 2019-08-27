// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)
#ifndef CORE_CLIENT_SYNC_READER_WRITER_H
#define CORE_CLIENT_SYNC_READER_WRITER_H

#include <cstdint>  // for int64_t
#include <memory>   // for shared_ptr
#include <string>

#include "../../core/client/channel_sptr.h"    // for ChannelSptr
#include "../../core/common/support/config.h"  // for GRPC_FINAL

namespace core {

class Status;
class ClientSyncReaderWriterImpl;

// Copyable.
class ClientSyncReaderWriter GRPC_FINAL {
 public:
  ClientSyncReaderWriter(const ChannelSptr& channel, const std::string& method,
                         int64_t timeout_ms);

 public:
  bool Write(const std::string& request) const;

  // Optional. Writing is auto closed in dtr().
  // Redundant calls are ignored.
  void CloseWriting();

  bool ReadOne(std::string* response) const;

  Status RecvStatus() const;

  // Set error status to break reading. Such as when parsing message failed.
  void SetErrorStatus(const Status& error_status);

 private:
  using Impl = ClientSyncReaderWriterImpl;
  const std::shared_ptr<Impl> impl_sptr_;
};  // class ClientSyncReaderWriter

// Todo: SyncGetInitMd();
// Todo: same as ClientReader?

}  // namespace core
#endif  // CORE_CLIENT_CLIENT_SYNC_READER_WRITER_H
