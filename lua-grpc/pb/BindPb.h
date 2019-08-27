/*
 * BindPb.h
 *
 *  Created on: Jul 23, 2019
 *      Author: root
 */

#ifndef PB_BINDPB_H_
#define PB_BINDPB_H_

namespace LuaIntf {
class LuaRef;
}

extern "C"
{
struct lua_State;
}

namespace pb
{

extern "C" void BindPb(const LuaIntf::LuaRef& mod);

}

#endif /* PB_BINDPB_H_ */
