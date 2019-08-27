#ifndef CORE_COMMON_BYTE_BUFFER_TO_STRING_H
#define CORE_COMMON_BYTE_BUFFER_TO_STRING_H

#include <string>

struct grpc_byte_buffer;

namespace core {

bool ByteBufferToString(grpc_byte_buffer& from, std::string& to);

}  // namespace core
#endif  // CORE_COMMON_BYTE_BUFFER_TO_STRING_H
