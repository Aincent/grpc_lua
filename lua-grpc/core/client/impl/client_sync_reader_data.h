#ifndef CORE_CLINET_IMPL_CLIENT_SYNC_READER_DATA_H
#define CORE_CLINET_IMPL_CLIENT_SYNC_READER_DATA_H

#include <string>

#include "../../../core/client/msg_str_cb.h"  // for MsgStrCb
#include "../../../core/client/status_cb.h"  // for StatusCb
#include "../../../core/common/impl/cqueue_for_pluck_sptr.h"  // for CQueueForPluckSptr
#include "../../common/call_sptr.h"   // for CallSptr
#include "../../common/status.h"                      // for Status

namespace core {

// Todo: Delete it.

// Wrap all data in shared struct pointer to make copy quick.
struct ClientSyncReaderData {
    CQueueForPluckSptr cq4p_sptr;
    CallSptr call_sptr;
    Status status;

    MsgStrCb msg_cb;
    StatusCb status_cb;
};

}  // namespace core
#endif  // CORE_CLINET_IMPL_CLIENT_SYNC_READER_DATA_H
