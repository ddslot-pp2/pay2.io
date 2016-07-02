#include "login_out_req.hpp"
#include <iostream>
#include <thread>
#include "../thread_local_task.hpp"
//#include "worker_thread_md.hpp"

void login_out::login_req(int vthread_id, std::shared_ptr<user> user, const Json& payload) {  
  std::thread::id this_id = std::this_thread::get_id();
  std::cout << "worker thread id: " << this_id << std::endl;
  std::cout << "virtual worker thread id: " << vthread_id << std::endl;

  auto local_task = std::make_shared<thread_local_task>();
  for(auto i=0; i<worker_thread_count; ++i) {
    // 유저가 게임에 접속함
    auto t = make_task(&thread_local_task::join_user, local_task, i, user);
    thread_local_info_md::get().push(i, t);

    

  } 
}

//static bool login_req(std::shared_ptr<cd_user> user, Json payload);
