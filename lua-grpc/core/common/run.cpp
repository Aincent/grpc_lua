// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../core/common/run.h"

#include "../../core/common/impl/do_next_completion.h"  // for DoNextCompletion()

namespace core {

void Run(CompletionQueueForNext& cq4n) {
	while (DoNextCompletion(cq4n))
	;
}

void UnBlockingRun(CompletionQueueForNext& cq4n) {
	DoTryNextCompletion(cq4n);
}

void UnBlockingRun(const CompletionQueueForNextSptr& cq4n_sptr) {
	  if (cq4n_sptr)
		  UnBlockingRun(*cq4n_sptr);
}

void Run(const CompletionQueueForNextSptr& cq4n_sptr) {
  if (cq4n_sptr)
	  Run(*cq4n_sptr);
}

}  // namespace core
