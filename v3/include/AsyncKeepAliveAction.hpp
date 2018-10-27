#ifndef __ASYNC_KEEPALIVEACTION_HPP__
#define __ASYNC_KEEPALIVEACTION_HPP__

#include <grpc++/grpc++.h>
#include "proto/rpc.grpc.pb.h"
#include "v3/include/Action.hpp"
#include "v3/include/AsyncKeepAliveResponse.hpp"
#include "etcd/Response.hpp"

using grpc::ClientAsyncReaderWriter;
using etcdserverpb::LeaseKeepAliveRequest;
using etcdserverpb::LeaseKeepAliveResponse;

namespace etcdv3 
{

  class AsyncKeepAliveAction : public etcdv3::Action
  {
    public:
      AsyncKeepAliveAction(etcdv3::ActionParameters param);
      AsyncKeepAliveResponse ParseResponse();
      void waitForResponse();
    private:
      LeaseKeepAliveResponse reply;
      std::unique_ptr<ClientAsyncReaderWriter<LeaseKeepAliveRequest,LeaseKeepAliveResponse>> stream;
  };

}

#endif
