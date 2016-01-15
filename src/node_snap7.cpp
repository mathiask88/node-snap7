/*
 * Copyright (c) 2016, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#include <node_snap7_client.h>

namespace node_snap7 {

NAN_MODULE_INIT(InitAll) {
  S7Client::Init(target);
}

NODE_MODULE(node_snap7, InitAll)

}  // namespace node_snap7
