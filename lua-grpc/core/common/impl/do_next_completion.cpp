// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../../core/common/impl/do_next_completion.h"

#include <grpc/impl/codegen/grpc_types.h>  // for grpc_event

#include "../../../core/common/completion_queue_for_next.h"  // for CompletionQueueForNext
#include "../../../core/common/impl/cqtag/completion_queue_tag.h"  // for CompletionQueueTag

namespace core {

// Blocking go next and complete.
// Return false if shutdown.
bool DoNextCompletion(CompletionQueueForNext& cq4n) {
   grpc_event ev = cq4n.Next();
    switch (ev.type) {
      case GRPC_OP_COMPLETE: {
        auto* tag = static_cast<CompletionQueueTag*>(ev.tag);
        assert(tag);
        tag->DoComplete(0 != ev.success);
        delete tag;  // created in RequestMethodCall()
        break;
      }  // case
      case GRPC_QUEUE_SHUTDOWN:
        return false;
      case GRPC_QUEUE_TIMEOUT:
//        assert(false);
        break;
      default:
        assert(false);
        break;
    }  // switch
    return true;
}

// UnBlocking go next and complete.
// Return false if shutdown.
bool DoTryNextCompletion(CompletionQueueForNext& cq4n) {
	grpc_event ev = cq4n.TryNext();
	switch (ev.type) {
	  case GRPC_OP_COMPLETE: {
		auto* tag = static_cast<CompletionQueueTag*>(ev.tag);
		assert(tag);
		tag->DoComplete(0 != ev.success);
		delete tag;  // created in RequestMethodCall()
		break;
	  }  // case
	  case GRPC_QUEUE_SHUTDOWN:
		return false;
	  case GRPC_QUEUE_TIMEOUT:
		break;
	  default:
		assert(false);
		break;
	}  // switch
	return true;
}

}  // namespace grpv_cb
