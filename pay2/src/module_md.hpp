#ifndef MODULE_MD
#define MODULE_MD

#include "declare.hpp"

//using namespace std;

class user_md;
class module_md {

public:

  static module_md& get() {
    static module_md obj;
    return obj;
  }

  auto get_user_md_ptr() { return user_md_ptr_; }
  
  shared_ptr<user_md> user_md_ptr_;
};

#endif

