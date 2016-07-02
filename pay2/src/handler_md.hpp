#ifndef HANDLER_MD_HPP
#define HANDLER_MD_HPP

#include <functional>
#include <vector>
#include "utils/json11.hpp"
#include "handler_md.hpp"
//#include "user.hpp"
using namespace json11;

class user;
using handler_callback = std::function<void(int vthread_id, std::shared_ptr<user> user, const Json& playload)>;

struct packet_info {
  packet_info(std::shared_ptr<user> _user, Json _payload) {
    user_ptr = _user;
    payload = _payload;
  }

  std::shared_ptr<user> user_ptr;
  Json payload;
};

class handler_md {
public:  
  handler_md();
  ~handler_md();

  static handler_md& get() {
    static handler_md obj;
    return obj;
  }

  void add_handler(handler_callback callback);
  void init();
  bool is_proper_payload(int packet_id);

  std::vector<handler_callback> handler_container_;
};

#endif
