#ifndef LOGININ_OUT_REQ_HPP
#define LOGININ_OUT_REQ_HPP

#include <memory>
#include "../utils/json11.hpp"

using namespace json11;

class user;

class login_out {
public:
  static void login_req(int vthread_id, std::shared_ptr<user> user, const Json& payload);
};

#endif
