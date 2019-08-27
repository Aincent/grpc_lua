// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../core/client/service_stub.h"
#include "../../core/client/impl/cqtag/client_async_call_cqtag.h"  // for ClientAsyncCallCqTag
#include "../../core/client/impl/cqtag/client_call_cqtag.h"        // for ClientCallCqTag
#include "../../core/common/completion_queue_for_next.h"  // for CompletionQueueForNext
#include "../../core/common/impl/call.h"
#include "../../core/common/impl/cqueue_for_pluck.h"        // for CQueueForPluck
#include "../../core/common/run.h"  // for Run()
#include "../common/completion_queue_for_next.h"  // to convert *cq4n_sptr_ to CompletionQueue in MakeSharedCall()

namespace core {

// default empty error callback
ErrorCb ServiceStub::default_error_cb_;

ServiceStub::ServiceStub(
    const ChannelSptr& channel_sptr,
    const CompletionQueueForNextSptr& cq4n_sptr /* = nullptr */)
    : start_(false),
      channel_sptr_(channel_sptr),  // copy shared_ptr
      cq4n_sptr_(cq4n_sptr),
      error_cb_(default_error_cb_),
      call_timeout_ms_sptr_(
          new std::atomic<int64_t>(channel_sptr->GetCallTimeoutMs())) {
  assert(channel_sptr);
  // Use an internal cq if no external cq.
  if (!cq4n_sptr_) cq4n_sptr_.reset(new CompletionQueueForNext);
  m_pID = new pthread_t();
}

ServiceStub::~ServiceStub() {
	assert(cq4n_sptr_);
	if(m_pID) {
		void *status = 0;
		pthread_join(*m_pID, &status);
		delete m_pID;
	}
}

Status ServiceStub::SyncRequest(const string& method, const string& request,
                                    string& response) {
  CQueueForPluck cq4p;
  CallSptr call_sptr(MakeSharedCall(method, cq4p));
  ClientCallCqTag tag(call_sptr);
  if (!tag.Start(request))
    return Status::InternalError("Failed to request.");
  cq4p.Pluck(&tag);
  return tag.GetResponse(response);
}

void ServiceStub::AsyncRequest(const string& method, const string& request,
                               const RespStrCb& response_cb,
                               const ErrorCb& error_cb) {
  CallSptr call_sptr(MakeSharedCall(method));
  using CqTag = ClientAsyncCallCqTag;
  CqTag* tag = new CqTag(call_sptr);
  tag->SetResponseCb(response_cb);
  tag->SetErrorCb(error_cb);
  if (tag->Start(request))
    return;

  delete tag;
  if (error_cb)
    error_cb(Status::InternalError("Failed to async request."));
}

// Blocking run stub.
void ServiceStub::Run() {
  assert(cq4n_sptr_);
  core::Run(cq4n_sptr_);

}

void ServiceStub::Shutdown() {
  assert(cq4n_sptr_);
  start_ = false;
  cq4n_sptr_->Shutdown();
}

CallSptr ServiceStub::MakeSharedCall(const string& method) const {
  assert(cq4n_sptr_);
  return MakeSharedCall(method, *cq4n_sptr_);
}

CallSptr ServiceStub::MakeSharedCall(const string& method, CompletionQueue& cq) const {
  return GetChannel().MakeSharedCall(method, cq, GetCallTimeoutMs());
}

grpc_connectivity_state ServiceStub::GetChannelStatus()
{
	  return GetChannel().OnCheckConnectivityState();
}

void *ServiceStub::ThreadRun(void* arg)
{
	ServiceStub* p = (ServiceStub*)arg;
	grpc_connectivity_state last_observed = GRPC_CHANNEL_SHUTDOWN;
	while(p->start_)
	{
		bool flag = p->GetChannel().WaitForStateChangeImpl(last_observed);
		last_observed =  p->GetChannelStatus();
		p->GetStatusChangeCb()(flag, last_observed);
	}

	return NULL;
}

}  // namespace core

