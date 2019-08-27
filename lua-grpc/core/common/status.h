/*
 *
 * Copyright 2015, Google Inc.
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

#ifndef CORE_STATUS_H
#define CORE_STATUS_H

#include <string>

#include <grpc/status.h>

#include "../../core/common/support/core_api.h"  // for CORE_API

namespace core {

// Non-thread-safe.
class CORE_API Status {
 public:
  /// Construct an OK instance.
  Status() : core_(GRPC_STATUS_OK) {}

  /// Construct an instance with associated \a core and \a details (also
  /// referred to as "error_message").
  Status(grpc_status_code core, const std::string& details)
      : core_(core), details_(details) {}

  // Pre-defined special status objects.
  /// An OK pre-defined instance.
  static const Status& OK;
  /// A CANCELLED pre-defined instance.
  static const Status& CANCELLED;
  /// An UNIMPLEMENTED pre-defined instance.
  static const Status& UNIMPLEMENTED;

 public:
  static Status InternalError(const std::string& details);

 public:
  /// Return the instance's error core.
  grpc_status_code GetCode() const { return core_; }
  /// Return the instance's error message.
  std::string GetDetails() const { return details_; }

  // Todo: change to operator bool()
  /// Is the status OK?
  bool ok() const { return core_ == GRPC_STATUS_OK; }

 public:
  void SetInternalError(const std::string& error_msg);

 private:
  grpc_status_code core_;
  std::string details_;
};

}  // namespace core

#endif  // CORE_STATUS_H
