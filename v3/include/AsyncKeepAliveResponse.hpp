#ifndef __ASYNC_KEEPALIVERESPONSE_HPP__
#define __ASYNC_KEEPALIVERESPONSE_HPP__

#include <grpc++/grpc++.h>
#include "proto/rpc.grpc.pb.h"
#include "proto/rpc.pb.h"
#include "v3/include/V3Response.hpp"

using etcdserverpb::LeaseKeepAliveRequest;
using etcdserverpb::LeaseKeepAliveResponse;
using etcdserverpb::KV;

namespace etcdv3
{
  class AsyncKeepAliveResponse : public etcdv3::V3Response
  {
    public:
      AsyncKeepAliveResponse(){};
      void ParseResponse(LeaseKeepAliveResponse &resp);
  };
}


#endif
