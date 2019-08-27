/*
 * ServerReader.h
 *
 *  Created on: Jul 23, 2019
 *      Author: root
 */

#ifndef LUA_SERVER_IMPL_SERVERREADER_H_
#define LUA_SERVER_IMPL_SERVERREADER_H_

#include "../../../core/server/server_reader.h"  // for core::ServerReader

#include "../../common/LuaRefFwd.h"  // forward LuaRef

#include <string>

namespace core {
class Status;
}  // namespace core

namespace impl {

struct LuaReaderFunctions;

// ServerReader is the interface of client streaming handler,
//  for both client-side streaming and bi-directional streaming.
// Thread-safe.
class ServerReader : public core::ServerReader {
 public:
  using LuaRef = LuaIntf::LuaRef;
  explicit ServerReader(const LuaRef& luaReader);
  virtual ~ServerReader();

 public:
  core::Status OnMsgStr(const std::string& msg_str) override;
  void OnError(const core::Status& status) override;
  void OnEnd() override;

 private:
  void InitLuaReaderFunctions(const LuaRef& luaReader);

 private:
  std::unique_ptr<LuaRef> m_pLuaReader;
};  // class ServerReader

}  // namespace impl

#endif /* LUA_SERVER_IMPL_SERVERREADER_H_ */
