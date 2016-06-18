//#include "log4cxx_md.hpp"
//#include "cd_handler_md.hpp"
//#include "cd_user_md.hpp"
//#include "db_md.hpp"
#include <memory>
#include <time.h>
#include "module_md.hpp"
#include "user_md.hpp"
#include "cards.hpp"

int main() {

  cards c;
  c.shuffle();

  using namespace std;
  using namespace json11;

  WsServer server(8080, 8);
  
  auto user_md_ptr = make_shared<user_md>();


  auto& echo = server.endpoint["^/echo/?$"];
  echo.onmessage = [&server](std::shared_ptr<WsServer::Connection> connection, std::shared_ptr<WsServer::Message> message) {

      //std::cout << connection->cd_user_ptr->name << std::endl;

      auto message_str = message->string();
      std::cout << "[packet]: " << message_str << std::endl;
      string err;
      auto packet = Json::parse(message_str, err);

      if (!err.empty()) {
	std::cout<< "[error] fail to parse json " << err.c_str() << std::endl;
      } else {
	//std::string h = packet["type"].string_value();

	/*
	if ( cd_handler_md::get().m.find(h) == cd_handler_md::get().m.end() ) {
	  std::cout << "[error] 핸들러 없음" << std::endl;
	  server.send_close(connection, 2);
	} else {
	  
	  bool r = cd_handler_md::get().m[h](connection->cd_user_ptr, payload);
	  if(!r) {
	    std::cout << "[error] 핸들러 콜백 처리중" << std::endl;
	  }
	}
	*/
      }
    };

  echo.onopen = [&](std::shared_ptr<WsServer::Connection> connection) {
    std::cout << "Server: Opened connection " << (size_t)connection.get() << std::endl;
    auto user_ptr = make_shared<user>(server);
    user_ptr->connection_ = connection;
    connection->user_ptr = user_ptr;
  };

  echo.onclose = [](std::shared_ptr<WsServer::Connection> connection, int status, const string& reason) {

   
    cout << "Server: Closed connection " << (size_t)connection.get() << " with status code " << status << endl;
  };

  echo.onerror = [](std::shared_ptr<WsServer::Connection> connection, const boost::system::error_code& ec) {
    cout << "Server: Error in connection " << (size_t)connection.get() << ". " << 
    "Error: " << ec << ", error message: " << ec.message() << endl;
  };
    

    thread server_thread([&server](){
        server.start();
    });

    this_thread::sleep_for(chrono::seconds(1));    
    server_thread.join();

  return 0;
}
