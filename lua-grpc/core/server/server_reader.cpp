// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../core/server/server_reader.h"

#include <cassert>

#include "../../core/server/impl/cqtag/server_reader_cqtag.h"         // for ServerReaderCqTag

namespace core {

Status ServerReader::OnMsgStr(const std::string& msg_str) {
  return Status::OK;
}

void ServerReader::Start(const CallSptr& call_sptr) {
  assert(call_sptr);

  using CqTag = ServerReaderCqTag;
  CqTag* tag = new CqTag(call_sptr, shared_from_this());
  if (tag->Start()) return;

  delete tag;
  OnError(Status::InternalError("Failed to init client streaming."));
  OnEnd();
}

}  // namespace core
