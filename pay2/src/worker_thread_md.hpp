#ifndef WORKER_THREAD_MD_HPP
#define WORKER_THREAD_MD_HPP

#include <functional>
#include <vector>
#include <thread>
#include <atomic>
#include <memory>
#include <functional>
#include "tbb/concurrent_queue.h"
#include "tbb/concurrent_vector.h"
#include "utils/json11.hpp"
#include "handler_md.hpp"
#include "thread_local_task.hpp"

using namespace json11;
using namespace tbb;

using task = std::function<void()>;

struct worker_thread_info {
  int id;
};


class worker_thread_md {
public:  
  worker_thread_md();
  ~worker_thread_md();

  static worker_thread_md& get() {
    static worker_thread_md obj;
    return obj;
  }


  void init();

  std::atomic<bool> is_run;
  std::vector<std::thread> thread_container;
  //concurrent_vector<thread_local_info> thread_local_info_container;
  concurrent_queue<std::shared_ptr<packet_info>> payload_queue;
};

#endif
