// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)
#ifndef CORE_SERVER_SERVER_READER_H
#define CORE_SERVER_SERVER_READER_H

#include <string>

#include "../../core/common/call_sptr.h"	  // for CallSptr

namespace core {

class Status;

// ServerReader is the interface of client streaming handler,
//  for both client-side streaming and bi-directional streaming.
// Thread-safe.
class ServerReader : public std::enable_shared_from_this<ServerReader> {
 public:
  ServerReader() {}
  virtual ~ServerReader() {}

 public:
  // OnMsgStr() may return parsing error.
  virtual Status OnMsgStr(const std::string& msg_str);
  virtual void OnError(const Status& status) {}
  // In case of end on error, OnError() is already called before OnEnd().
  virtual void OnEnd() {}

 public:
  // XXX need document...
  // Start the reader.
  void Start(const CallSptr& call_sptr);
};  // class ServerReader

}  // namespace core
#endif  // CORE_SERVER_SERVER_READER_H
