/*
 * Copyright (c) 2019, Mathias Küsel
 * MIT License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#include <node_snap7_client.h>
//#include <node_snap7_server.h>

namespace node_snap7 {

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  S7Client::Init(env, exports);
  //S7Server::Init(env, exports);
  
  return exports;
}

NODE_API_MODULE(node_snap7, InitAll)

}  // namespace node_snap7
