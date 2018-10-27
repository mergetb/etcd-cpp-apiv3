#include <catch.hpp>

#include "etcd/Client.hpp"

static std::string etcd_uri("http://127.0.0.1:2379");

TEST_CASE("keepalive")
{
  etcd::Client etcd(etcd_uri);
  etcd.rmdir("/test", true).wait();

  etcd::Response lease = etcd.leasegrant(5).get();
  REQUIRE(true == lease.is_ok());

  std::string key = "/test/muffin",
              value = "blueberry";

  std::cout << "creating initial key with lease" << std::endl;

  // create key with lease
  etcd::Response add = etcd.add(key, value, lease.value().lease()).get();
  REQUIRE(true == add.is_ok());

  // verify key already exists within lease lifetime
  REQUIRE("Key already exists" == etcd.add(key, value).get().error_message());

  std::cout << "waiting for key to expire" << std::endl;
  sleep(7);

  std::cout << "verifying key no longer exists" << std::endl;
  // verify key no longer exists beyond lease lifetime
  lease = etcd.leasegrant(5).get();
  REQUIRE(true == lease.is_ok());
  add = etcd.add(key, value, lease.value().lease()).get();
  REQUIRE(true == add.is_ok());

  std::cout << "wait a bit on new key" << std::endl;
  sleep(3);

  std::cout << "keepalive new key" << std::endl;
  etcd::Response keepalive = etcd.keepalive(lease.value().lease()).get();
  REQUIRE(true == keepalive.is_ok());

  sleep(3);

  std::cout << "verify keepalive'd key" << std::endl;
  // verify key already exists within the extended lease lifetime
  REQUIRE("Key already exists" == etcd.add(key, value).get().error_message());

}

