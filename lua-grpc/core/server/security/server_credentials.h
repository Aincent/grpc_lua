// Licensed under the Apache License, Version 2.0.

#ifndef CORE_SERVER_CREDENTIALS_H
#define CORE_SERVER_CREDENTIALS_H

struct grpc_server_credentials;

namespace core {

// Wrapper around \a grpc_server_credentials, a way to authenticate a server.
class ServerCredentials {
 public:
  virtual grpc_server_credentials* c_creds() const { return nullptr; }
  virtual ~ServerCredentials(){}
};

class InsecureServerCredentials : public ServerCredentials {
};

}  // namespace core

#endif  // CORE_SERVER_CREDENTIALS_H
