#include "../../../core/common/impl/byte_buffer_to_string.h"

#include <grpc/byte_buffer_reader.h>  // for grpc_byte_buffer_reader
#include <grpc/byte_buffer.h>  // for grpc_byte_buffer_reader_init()

#include "../../../core/common/impl/slice.h"  // for StringFromCopiedSlice()

namespace core {

bool ByteBufferToString(grpc_byte_buffer& from, std::string& to) {
  grpc_byte_buffer_reader reader;
  if (!grpc_byte_buffer_reader_init(&reader, &from)) {
    return false;
  }
  grpc_slice slice = grpc_byte_buffer_reader_readall(&reader);
  to = StringFromCopiedSlice(slice);
  grpc_slice_unref(slice);
  grpc_byte_buffer_reader_destroy(&reader);
  return true;
}

}  // namespace core

