/*
 * BindNet.cpp
 *
 *  Created on: Aug 20, 2019
 *      Author: root
 */

#include "BindNet.h"
#include <LuaIntf/LuaIntf.h>
#include <string>
#include "Net.h"

using namespace LuaIntf;

namespace net {

extern "C"
{

void BindNet(const LuaIntf::LuaRef& mod)
{
    LuaIntf::LuaBinding(mod)
        .addFunction("get_intranet_ip", &net::GetIntranetIp)
	;
}
}
}
