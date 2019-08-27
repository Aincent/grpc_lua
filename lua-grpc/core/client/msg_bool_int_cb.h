/*
 * msg_bool_int_cb.h
 *
 *  Created on: Aug 14, 2019
 *      Author: root
 */

#ifndef SRC_CORE_CLIENT_MSG_BOOL_INT_CB_H_
#define SRC_CORE_CLIENT_MSG_BOOL_INT_CB_H_

#include <functional>  // for std::function()

namespace core {

using WaitForStatusCb = std::function<void (const bool& status, const int& status_code)>;
using StatusChangeCb = WaitForStatusCb;

}  // namespace core

#endif /* SRC_CORE_CLIENT_MSG_BOOL_INT_CB_H_ */
