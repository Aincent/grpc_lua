/*
 * BindServer.h
 *
 *  Created on: Jul 23, 2019
 *      Author: root
 */

#ifndef LUA_SERVER_BINDSERVER_H_
#define LUA_SERVER_BINDSERVER_H_

#include "../common/LuaRefFwd.h"  // forward LuaRef

namespace server {
	void BindServer(const LuaIntf::LuaRef& mod);
}

#endif /* LUA_SERVER_BINDSERVER_H_ */
