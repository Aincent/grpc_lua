#ifndef CORE_CLINET_IMPL_CLIENT_SYNC_WRITER_DATA_H
#define CORE_CLINET_IMPL_CLIENT_SYNC_WRITER_DATA_H

#include <memory>  // for shared_ptr<>
#include <string>

#include "../../../core/common/impl/cqueue_for_pluck_sptr.h"  // for CQueueForPluckSptr
#include "../../../core/common/status.h"  // for Status
#include "../../common/call_sptr.h"  // for CallSptr

namespace core {

// Todo: Delete it.

// Wrap all data in shared struct pointer to make copy quick.
struct ClientSyncWriterData {
  CQueueForPluckSptr cq4p_sptr;
  CallSptr call_sptr;
  Status status;
};

}  // namespace core
#endif  // CORE_CLINET_IMPL_CLIENT_SYNC_WRITER_DATA_H
