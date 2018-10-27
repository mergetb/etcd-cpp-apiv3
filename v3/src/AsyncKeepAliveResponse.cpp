#include "v3/include/AsyncKeepAliveResponse.hpp"
#include "v3/include/action_constants.hpp"

void etcdv3::AsyncKeepAliveResponse::ParseResponse(LeaseKeepAliveResponse &resp)
{
  index = resp.header().revision();
  value.kvs.set_lease(resp.id());
  value.set_ttl(resp.ttl());
}
