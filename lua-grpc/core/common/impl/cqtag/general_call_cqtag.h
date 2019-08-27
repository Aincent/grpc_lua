// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_IMPL_GENERAL_CALL_CQTAG_H
#define CORE_IMPL_GENERAL_CALL_CQTAG_H

#include <cassert>  // for assert()

#include "../../../../core/common/impl/complete_cb.h"             // for CompleteCb
#include "../../../../core/common/impl/cqtag/call_cqtag.h"                          // for CallCqTag
#include "../../support/config.h"  // for GRPC_OVERRIDE

namespace core {

// Call completion queue tag with callback on completion.
class GeneralCallCqTag : public CallCqTag {
 public:
  explicit GeneralCallCqTag(const CallSptr& call_sptr) : CallCqTag(call_sptr) {
    assert(call_sptr);
  }

 public:
  void SetCompleteCb(const CompleteCb& complete_cb) {
    complete_cb_ = complete_cb;
  }

  void DoComplete(bool success) GRPC_OVERRIDE {
    if (complete_cb_)
      complete_cb_(success);
  }

 private:
  CompleteCb complete_cb_;
};  // class GeneralCallCqTag

}  // namespace core

#endif  // CORE_IMPL_GENERAL_CALL_CQTAG_H
