/*
 * Net.h
 *
 *  Created on: Aug 20, 2019
 *      Author: root
 */

#ifndef SRC_NET_NET_H_
#define SRC_NET_NET_H_

#include <LuaIntf/LuaIntf.h>

namespace net {
LuaIntf::LuaRef GetIntranetIp (lua_State* L);
}

#endif /* SRC_NET_NET_H_ */
