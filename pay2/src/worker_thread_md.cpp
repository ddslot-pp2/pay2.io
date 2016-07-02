#include "worker_thread_md.hpp"
//#include "thread_local_task.hpp"
#include <iostream>


void worker_run(worker_thread_info info) {

  auto vthread_id = info.id;
  std::cout << "virtual thread id:" << vthread_id << std::endl;
    
  concurrent_queue<std::shared_ptr<packet_info>>& payload_queue = worker_thread_md::get().payload_queue;

  concurrent_vector<thread_local_info>& thread_local_info_container = thread_local_info_md::get().info_container;
  
  while(worker_thread_md::get().is_run) {

    std::shared_ptr<packet_info> _packet_info;
    while(payload_queue.try_pop(_packet_info)) {
      auto packet_id = _packet_info->payload["packet_id"].int_value();
      auto handler = handler_md::get().handler_container_[packet_id];

      std::thread::id this_id = std::this_thread::get_id();
      std::cout << "worker thread id: " << this_id << std::endl;

      handler(vthread_id, _packet_info->user_ptr, _packet_info->payload);
      
      printf("payload in queue");
    }

   

    std::function<void()> local_task;
    while(thread_local_info_container[vthread_id].task_queue.try_pop(local_task)) {
      local_task();
    }
    
   
  
  }

}

worker_thread_md::worker_thread_md() {
  is_run = true;
}

worker_thread_md::~worker_thread_md() {

}

void worker_thread_md::init() {
  for(auto i=0; i<worker_thread_count; ++i) {
    worker_thread_info info;
    info.id = thread_container.size();
    
    thread_local_info_md::get().info_container.push_back(thread_local_info());
    thread_container.push_back(std::thread(worker_run, info));
  }

  for(std::thread& thread : thread_container) {
    thread.join();
  }
}
