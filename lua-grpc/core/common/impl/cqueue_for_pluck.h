// Completion queue for pluck.

#ifndef CORE_IMPL_CQUEUE_FOR_PLUCK_H
#define CORE_IMPL_CQUEUE_FOR_PLUCK_H

#include "../../../core/common/completion_queue.h"  // for CompletionQueue
#include "../../../core/common/impl/cqueue_for_pluck_sptr.h"
#include "../../../core/common/support/time.h"  // for TimePoint<>

namespace core {

/// A thin wrapper around grpc_completion_queue for pluck.
/// Thread-safe.
class CQueueForPluck : public CompletionQueue {
 public:
  /// Default constructor. Implicitly creates a \a grpc_completion_queue
  /// instance.
  CQueueForPluck();

  /// Wrap \a take, taking ownership of the instance.
  ///
  /// \param take The completion queue instance to wrap. Ownership is taken.
  explicit CQueueForPluck(grpc_completion_queue* take);

  /// Destructor. Destroys the owned wrapped completion queue / instance.
  virtual ~CQueueForPluck() GRPC_OVERRIDE;

 public:
  /// Wraps \a grpc_completion_queue_pluck.
  /// \warning Must not be mixed with calls to \a Next.
  template <typename T>
  grpc_event Pluck(void* tag, const T& deadline) {
    TimePoint<T> deadline_tp(deadline);
    return PluckInternal(tag, deadline_tp.raw_time());
  }

  /// Wraps \a grpc_completion_queue_pluck.
  /// \warning Must not be mixed with calls to \a Next.
  grpc_event Pluck(void* tag) {
    return PluckInternal(tag, gpr_inf_future(GPR_CLOCK_REALTIME));
  }

  /// Performs a single polling pluck on \a tag.
  grpc_event TryPluck(void* tag) {
    return PluckInternal(tag, gpr_time_0(GPR_CLOCK_REALTIME));
  }

 private:
  grpc_event PluckInternal(void* tag, gpr_timespec deadline);
};

}  // namespace core

#endif  // CORE_IMPL_CQUEUE_FOR_PLUCK_H
