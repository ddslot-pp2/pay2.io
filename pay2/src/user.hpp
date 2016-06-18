#ifndef USER_HPP
#define USER_HPP

#include <memory>
#include "server_ws.hpp"
#include "utils/json11.hpp"
//typedef SocketServer<WS> WsServer;
using WsServer = SocketServer<WS>;

class user : public std::enable_shared_from_this<user> { 
  
public:
  user(WsServer& server);
  user(WsServer& server, std::shared_ptr<WsServer::Connection> connection_ptr);
  ~user();

  virtual void destory();

  void send(std::string payload);
  void send2(json11::Json payload);


  void start_alive_time();
  void update_alive_t() { alive_t_ = time(NULL); }
  time_t get_alive_t() { return alive_t_; }

  int get_uid() { return uid_; }
  void set_uid(int uid) { uid_ = uid; }


  WsServer& server_;
  std::weak_ptr<WsServer::Connection> connection_;

private:
  int uid_;
  size_t alive_t_;
};

#endif
