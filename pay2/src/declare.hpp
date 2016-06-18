#ifndef DECLARE_HPP
#define DECLARE_HPP

#include <memory>

template<class Type>
using shared_ptr = std::shared_ptr<Type>;

class user;
using user_sp = shared_ptr<user>;

#endif
