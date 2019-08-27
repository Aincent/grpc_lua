// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_COMPLETION_QUEUE_TAG_H
#define CORE_COMPLETION_QUEUE_TAG_H

namespace core {

// Interface of core completion queue tag. CqTag.
class CompletionQueueTag {
 public:
  virtual ~CompletionQueueTag() {}
  virtual void DoComplete(bool success) {};
};

}  // namespace core

#endif  // CORE_COMPLETION_QUEUE_TAG_H
