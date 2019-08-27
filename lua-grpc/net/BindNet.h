/*
 * BindNet.h
 *
 *  Created on: Aug 20, 2019
 *      Author: root
 */

#ifndef SRC_NET_BINDNET_H_
#define SRC_NET_BINDNET_H_


namespace LuaIntf {
class LuaRef;
}

extern "C"
{
struct lua_State;
}

namespace net
{

extern "C" void BindNet(const LuaIntf::LuaRef& mod);

}



#endif /* SRC_NET_BINDNET_H_ */
