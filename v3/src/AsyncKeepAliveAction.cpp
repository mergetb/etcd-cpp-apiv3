#include "v3/include/AsyncKeepAliveAction.hpp"
#include "v3/include/action_constants.hpp"

etcdv3::AsyncKeepAliveAction::AsyncKeepAliveAction(
    etcdv3::ActionParameters param)
  : etcdv3::Action(param)
{
  stream = parameters.lease_stub->AsyncLeaseKeepAlive(
      &context, &cq_, (void*)"keepalive"
  );

  // wait for stream to become ready
  void *got_tag;
  bool ok{false};
  cq_.Next(&got_tag, &ok);

  LeaseKeepAliveRequest keepalive_req;
  keepalive_req.set_id(param.lease_id);

  stream->Write(keepalive_req, (void*)"write");
  stream->Read(&reply, (void*)this);
}

void etcdv3::AsyncKeepAliveAction::waitForResponse()
{
  void* got_tag;
  bool ok = false;

  while(cq_.Next(&got_tag, &ok))
  {
    if(ok == false || (got_tag == (void*)"writes done"))
    {
      break;
    }
    if(got_tag == (void*)this) // read tag
    {
      if(reply.id() && reply.ttl())
      {
        stream->WritesDone((void*)"writes done");
      }
      else
      {
        stream->Read(&reply, (void*)this);
      }
    }
  }
}

etcdv3::AsyncKeepAliveResponse etcdv3::AsyncKeepAliveAction::ParseResponse()
{
  AsyncKeepAliveResponse keepalive_resp;
  if(!status.ok())
  {
    keepalive_resp.set_error_code(status.error_code());
  }
  else
  {
    keepalive_resp.ParseResponse(reply);
  }
  return keepalive_resp;
}
