#include <memory>
#include <atomic>
#include <iostream>
#include <vector>
#include "tbb/concurrent_queue.h"
#include "tbb/concurrent_vector.h"
#include "user.hpp"

using namespace tbb;

const static int worker_thread_count = 4;

struct thread_local_info {
  concurrent_queue<std::function<void()>> task_queue;
  std::vector<std::shared_ptr<user>> user_container;
};

class thread_local_info_md {
public:

  thread_local_info_md() {
  }


   static thread_local_info_md& get() {
    static thread_local_info_md obj;
    return obj;
  }

  void push(int index, std::function<void()> task) {
    info_container[index].task_queue.push(task);
  }


  concurrent_vector<thread_local_info> info_container;
};

class thread_local_task {
public:
  thread_local_task() {
    execute_count = 0;
  }

  virtual ~thread_local_task() {}


  void join_user(int vtid, std::shared_ptr<user> user) {
    
    std::cout << "called join_user id: " << vtid << std::endl;
    std::cout << "execute count: " << ++execute_count << std::endl;

    if(user) {
      thread_local_info_md::get().info_container[vtid].user_container.push_back(user);
    }

    // 모든 스레드 전파 완료
    if(execute_count == worker_thread_count) {
      for(auto& user : thread_local_info_md::get().info_container[vtid].user_container) {
	json11::Json res = json11::Json::object {
	  { "type", "login_res" },
	  { "user size", std::to_string(thread_local_info_md::get().info_container[vtid].user_container.size()) }
	};
	user->send2(res);
      }
    }
  }

  std::atomic<int> execute_count;
};

template <typename Fn, class T, typename... Args>
std::function<void()> make_task(Fn fn, std::shared_ptr<T> obj, Args... args) {
  return std::bind(fn, obj, std::forward<Args>(args)...);
}


/*
class insert_tank_zone : public thread_local_task {
public:
  void execute(int vthread_id);
  void execute_done();
  
  void execute(int vthread_id) {
    execute_count++;
    std::cout << "done done" << std::endl;
    worker_thread_md::get().thread_local_info_container[vthread_id].tmp_container.push_back(10);
    check_done();
  }

  void execute_done() {
    std::cout << "done done" << std::endl;
  }
  

};

*/
