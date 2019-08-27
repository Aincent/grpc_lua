// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../core/server/server.h"

#include <algorithm>  // for for_each()
#include <unistd.h>
#include <grpc/grpc.h>
#include <grpc/grpc_security.h>  // for grpc_server_add_secure_http2_port()

#include "../../core/common/completion_queue_for_next.h"  // for CompletionQueueForNext
#include "../../core/common/run.h"                   // for Run()
#include "../../core/server/impl/cqtag/server_method_call_cqtag.h"   // for ServerMethodCallCqTag
#include "../../core/server/security/server_credentials.h"  // for InsecureServerCredentials
#include "../../core/server/service.h"

namespace core {

Server::Server()
    : cq4n_sptr_(new CompletionQueueForNext),  // shared_ptr
      started_(false),
      shutdown_(false),
      c_server_uptr_(MakeUniqueGrpcServer()) {
  assert(cq4n_sptr_ && c_server_uptr_);
  grpc_server_register_completion_queue(c_server_uptr_.get(), &cq4n_sptr_->c_cq(),
                                        nullptr);
  pid_ = new pthread_t();
}

Server::~Server() { 
  Shutdown(); 
  if(pid_) {
    void *status = 0;
    pthread_join(*pid_, &status);
    delete pid_;
  }
}

static grpc_server_register_method_payload_handling GetMethodPayloadHandling(
    Service& service, size_t method_index) {
  assert(method_index < service.GetMethodCount());
  if (service.IsMethodClientStreaming(method_index))
    return GRPC_SRM_PAYLOAD_NONE;
  return GRPC_SRM_PAYLOAD_READ_INITIAL_BYTE_BUFFER;
}

void Server::RegisterService(Service& service) {
  RegisteredService& rs = service_map_[service.GetFullName()];
  rs.service = &service;
  RegisteredMethodVec& registered_methods = rs.registered_methods;
  registered_methods.clear();

  for (size_t i = 0; i < service.GetMethodCount(); ++i) {
    const std::string& name = service.GetMethodName(i);
    void* registered_method = grpc_server_register_method(
        c_server_uptr_.get(), name.c_str(), nullptr/* TODO: host*/,
        GetMethodPayloadHandling(service, i), 0);
    registered_methods.push_back(registered_method);  // maybe null
  }
}

void Server::RegisterService(const ServiceSptr& service_sptr) {
  if (!service_sptr) return;
  services_.push_back(service_sptr);
  RegisterService(*service_sptr);
}

int Server::AddListeningPort(const std::string& addr,
                             const ServerCredentials& creds) {
  assert(!started_);
  assert(c_server_uptr_);
  grpc_server_credentials* c_creds = creds.c_creds();
  if (c_creds) {
    return grpc_server_add_secure_http2_port(c_server_uptr_.get(), addr.c_str(),
                                             c_creds);
  } else {
    return grpc_server_add_insecure_http2_port(c_server_uptr_.get(),
                                               addr.c_str());
  }
}

int Server::AddListeningPort(const std::string& addr) {
  return AddListeningPort(addr, InsecureServerCredentials());
}

void Server::ShutdownInternal(gpr_timespec deadline) {
  if (!started_) return;
  if (shutdown_) return;
  shutdown_ = true;

  assert(cq4n_sptr_);
  grpc_server_shutdown_and_notify(c_server_uptr_.get(), &cq4n_sptr_->c_cq(), this);
  // XXX cq_sptr_->Pluck(this);
  cq4n_sptr_->Shutdown();
}

void Server::Run() {
 // assert(!started_);
 // assert(!shutdown_);
 assert(c_server_uptr_);
 if (shutdown_) return;
 if(!started_)
 {
    started_ = true;
    grpc_server_start(c_server_uptr_.get());
    RequestMethodsCalls();
 }

 core::UnBlockingRun(cq4n_sptr_);
 // pthread_create(pid_, NULL, &ThreadRun, this);
}

void *Server::ThreadRun(void* arg) {
  Server* p = (Server*)arg;
  while(!p->shutdown_)
  {
    core::UnBlockingRun(p->cq4n_sptr_);
    usleep(10);
  }

  return NULL;
}

void Server::RequestMethodsCalls() const {
  for (auto itr = service_map_.begin(); itr != service_map_.end(); ++itr) {
    const RegisteredService& rs = (*itr).second;
    RequestServiceMethodsCalls(rs);
  }
}

void Server::RequestServiceMethodsCalls(const RegisteredService& rs) const {
  assert(rs.service);
  const RegisteredMethodVec& rms = rs.registered_methods;
  for (size_t i = 0; i < rms.size(); ++i) {
    if (!rms[i]) continue;
    // Delete in Run(). Calls grpc_server_request_registered_call() in ctr().
    new ServerMethodCallCqTag(c_server_uptr_.get(), rs.service, i, rms[i],
                              cq4n_sptr_);
  }
}

Server::GrpcServerUptr Server::MakeUniqueGrpcServer() {
  grpc_channel_args channel_args{0, nullptr};
  grpc_server* svr = grpc_server_create(&channel_args, nullptr);
  return GrpcServerUptr(svr, grpc_server_destroy);
}

}  // namespace core
