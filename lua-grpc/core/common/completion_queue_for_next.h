// Complete queue for next.
#ifndef CORE_COMMON_COMPLETION_QUEUE_FOR_NEXT_H
#define CORE_COMMON_COMPLETION_QUEUE_FOR_NEXT_H

#include "../../core/common/completion_queue.h"  // for CompletionQueue
#include "../../core/common/support/time.h"      // for TimePoint

namespace core {

/// A thin wrapper around grpc_completion_queue for next.
/// Thread-safe.
class CompletionQueueForNext : public CompletionQueue {
 public:
  /// Default constructor. Implicitly creates a \a grpc_completion_queue
  /// instance.
  CompletionQueueForNext();

  /// Wrap \a take, taking ownership of the instance.
  ///
  /// \param take The completion queue instance to wrap. Ownership is taken.
  explicit CompletionQueueForNext(grpc_completion_queue* take);

  /// Destructor. Destroys the owned wrapped completion queue instance.
  virtual ~CompletionQueueForNext() GRPC_OVERRIDE;

 public:
  /// Wraps \a grpc_completion_queue_next.
  /// \param deadline[in] How long to block in wait for an event.
  template <typename T>
  grpc_event Next(const T& deadline) {
    TimePoint<T> deadline_tp(deadline);
    return NextInternal(deadline_tp.raw_time());
  }

  grpc_event Next() {
    return NextInternal(gpr_inf_future(GPR_CLOCK_REALTIME));
  }
  grpc_event TryNext() {
    return NextInternal(gpr_time_0(GPR_CLOCK_REALTIME));
  }

 private:
  grpc_event NextInternal(gpr_timespec deadline);
};

}  // namespace core
#endif  // CORE_COMMON_COMPLETION_QUEUE_FOR_NEXT_H
