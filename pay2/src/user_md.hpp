#ifndef USER_MD_HPP
#define USER_MD_HPP

#include "declare.hpp"
#include <set>
#include "user.hpp"

class user_md {
public:
  user_md() = default;
  ~user_md() = default;

  //private:
  std::set<user_sp> connections_;
  std::map<int, user_sp> users_;
};

#endif
