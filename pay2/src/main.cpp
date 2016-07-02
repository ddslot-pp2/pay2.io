 //#include "log4cxx_md.hpp"
//#include "cd_handler_md.hpp"
//#include "cd_user_md.hpp"
//#include "db_md.hpp"
#include <memory>
#include <time.h>
#include "module_md.hpp"
#include "user_md.hpp"
#include "handler_md.hpp"
#include "worker_thread_md.hpp"
#include "tbb/concurrent_queue.h"
#include "tbb/concurrent_hash_map.h"
#include <deque>
#define TBB_PREVIEW_MEMORY_POOL 1
#include "tbb/memory_pool.h"
#include <thread>

using namespace std;
using namespace tbb;

class base {
public:
  base() {
    std::cout << "base ctor called" << std::endl;
  }

  virtual ~base() {
    std::cout << "base dtor called" << std::endl;
  }
  int mp;
};

class player : public base {
public:
  int hp;
  float lv;
  int d;
  char c;
  //  int c;
  
  player() {
    std::cout << "called player ctor" << std::endl;
  }
  virtual ~player() {
    std::cout << "called player dtor" << std::endl;
  }
};

//tbb::memory_pool<std::allocator<char>> my_pool;
//tbb::memory_pool<tbb::scalable_allocator<char>> my_pool;
tbb::memory_pool<tbb::cache_aligned_allocator <char>> my_pool;

template <class T, typename... Args>
T* xnew(Args&&... arg) {
  std::cout << "called xnew function" << std::endl;
  void* alloc = my_pool.malloc(sizeof(T));
  new (alloc)T(std::forward<Args>(arg)...);
  return reinterpret_cast<T*>(alloc);
}

template <class T>
void xdelete(T* object) {
  object->~T();
  std::cout << "called xdelete function" << std::endl;
  my_pool.free(object);
}

template <class T, typename... Args> 
std::shared_ptr<T> xnew_shared_ptr(Args... args) {
  return std::shared_ptr<T>(xnew<T>(args...), xdelete<T>);
}



//순회가 가능하게 해야함
//using concurrent_hash_player_map = 



int main() {

  /*
  std::vector<std::thread> thread_container;

  std::cout << "start" << std::endl;

  std::chrono::steady_clock::time_point _start(std::chrono::steady_clock::now());

  for(auto t=0; t<8; ++t) {
  thread_container.push_back(std::thread([]{

	for(auto i=0; i<10000; ++i) {
	  auto p = xnew_shared_ptr<player>();
	  //auto p = std::make_shared<player>();
	  p->hp = 20;
	  p->mp = 30;	 

	  //a.push_back(p.get());
	}

      }));
  }
    
    
  for(auto& t : thread_container) {
    t.join();
  }

  std::chrono::steady_clock::time_point _end(std::chrono::steady_clock::now());
  
  std::cout << std::chrono::duration_cast<std::chrono::duration<double>>(_end - _start).count() << std::endl;
h
  std::cout << "end" << std::endl;
*/    

  /* exmaple A
  using player_pool_allocator = tbb::memory_pool_allocator<player>;
  tbb::memory_pool<std::allocator<char>> my_pool;
  std::list<player, player_pool_allocator> player_container((player_pool_allocator(my_pool)));

  for(auto i=0; i<10; ++i) {
    player p;
    p.hp = i;
    p.lv = 0.0f;
    player_container.emplace_back(std::move(p));
  }

  for(auto& player : player_container) {
    auto hp = player.hp;
    std::cout << "hp : " << hp << std::endl;
  }
  */

 
  using namespace std;
  using namespace json11;

  handler_md::get().init();
  std::thread worker_thread([]{
      worker_thread_md::get().init();
    });

  WsServer server(8080, 8);
  
  //auto user_md_ptr = make_shared<user_md>();


  auto& echo = server.endpoint["^/echo/?$"];
  echo.onmessage = [&server](std::shared_ptr<WsServer::Connection> connection, std::shared_ptr<WsServer::Message> message) {

    //std::cout << connection->cd_user_ptr->name << std::endl;

    auto msg = message->string();
    std::cout << "[payload]: " << msg << std::endl;
    string err;
    auto payload = Json::parse(std::move(msg), err);

    if (!err.empty()) {
      std::cout<< "[error] fail to parse json " << err.c_str() << std::endl;
    } else {
      
      std::thread::id this_id = std::this_thread::get_id();
      std::cout << "io thread id:" << this_id << std::endl;

      auto packet_id = payload["packet_id"].int_value();
      
      if(handler_md::get().is_proper_payload(packet_id)) {
	auto _packet_info = std::make_shared<packet_info>(connection->user_ptr, payload);
	worker_thread_md::get().payload_queue.push(_packet_info);
      }
      
      
	
      /*
      if ( cd_handler_md::get().m.find(h) == cd_handler_md::get().m.end() ) {
	std::cout << "[error] 핸들러 없음" << std::endl;
	server.send_close(connection, 2);
      } else {
	  
	//bool r = cd_handler_md::get().m[h](connection->cd_user_ptr, payload);
	if(!r) {
	  std::cout << "[error] 핸들러 콜백 처리중" << std::endl;
	}
      }
      */
	
    }
  };

  echo.onopen = [&](std::shared_ptr<WsServer::Connection> connection) {
    std::cout << "Server: Opened connection " << (size_t)connection.get() << std::endl;
    auto user_ptr = std::make_shared<user>(server);
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

  worker_thread.join();

  this_thread::sleep_for(chrono::seconds(1));    
  server_thread.join();

  return 0;
}
