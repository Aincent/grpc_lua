/*
 *
 * Copyright 2015-2016, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source core must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/// A completion queue implements a concurrent producer-consumer queue.

#ifndef CORE_COMPLETION_QUEUE_H
#define CORE_COMPLETION_QUEUE_H

#include <cassert>
#include <memory>  // for unique_ptr<>

#include "../../core/common/completion_queue_sptr.h"  // for CompletionQueueSptr
#include "../../core/common/support/config.h"  // for GRPC_OVERRIDE
#include "../../core/common/support/grpc_library.h"           // for GrpcLibrary

struct grpc_completion_queue;
struct grpc_event;

namespace core {

/// A thin wrapper around grpc_completion_queue (see grpc
///  src/core/surface/completion_queue.h).
/// Thread-safe.
class CompletionQueue : public GrpcLibrary {
 public:
  /// Wrap \a take, taking ownership of the instance.
  ///
  /// \param take The completion queue instance to wrap. Ownership is taken.
  explicit CompletionQueue(grpc_completion_queue* take);

  /// Destructor. Destroys the owned wrapped completion queue / instance.
  virtual ~CompletionQueue() GRPC_OVERRIDE;

 public:
  /// Request the shutdown of the queue.
  ///
  /// \warning This method must be called at some point. Once invoked, \a Next
  /// will start to return false and \a AsyncNext will return \a
  /// NextStatus::SHUTDOWN. Only once either one of these methods does that
  /// (that is, once the queue has been \em drained) can an instance of this
  /// class be destroyed.
  void Shutdown();

  /// Returns the underlying \a grpc_completion_queue
  /// instance.
  grpc_completion_queue& c_cq() const {
    assert(c_cq_uptr_);
    return *c_cq_uptr_;
  }

 private:
  const std::unique_ptr<grpc_completion_queue, void (*)(grpc_completion_queue*)>
      c_cq_uptr_;  // owned
};

}  // namespace core

#endif  // CORE_COMPLETION_QUEUE_H
