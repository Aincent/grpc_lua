// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_SERVICE_SPTR_H
#define CORE_SERVICE_SPTR_H

#include <memory>  // for shared_ptr<>

namespace core {

class Service;
using ServiceSptr = std::shared_ptr<Service>;

}  // namespace core

#endif  // CORE_SERVICE_SPTR_H
