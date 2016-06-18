#include "user.hpp"
#include <iostream>

//typedef std::shared_ptr<user> user_ptr;  

user::user(WsServer& server) : server_(server) {

}

user::user(WsServer& server, std::shared_ptr<WsServer::Connection> connection_ptr) : server_(server), connection_(connection_ptr) {
  std::cout << "유저 생성자 called" << std::endl;
  start_alive_time();
}

user::~user() {
  std::cout << "유저 소멸자 called" << std::endl;
}

void user::start_alive_time() {
  alive_t_ = time(NULL);
}

void user::send(std::string packet) {
  auto send_stream = std::make_shared<WsServer::SendStream>();
  *send_stream << packet;

  auto connection = connection_.lock(); 
  if(connection) server_.send(connection, send_stream);
}

void user::send2(json11::Json packet) {
  send(packet.dump());
}

void user::destory() {
  std::cout << "user destroy called" << std::endl;
  //auto sp = std::shared_ptr<user>(shared_from_this());

}
