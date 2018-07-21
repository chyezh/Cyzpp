#ifndef _CYZPP_THREAD_POOL_H__
#define _CYZPP_THREAD_POOL_H__

#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include "../basic/common.h"
#include "thread_safe.h"

CYZPP_BEGIN

template <class Task>
class ThreadPool {
 public:
  // member type
  typedef Task task_t;

 public:
  ThreadPool();

  ThreadPool(const ThreadPool &) = delete;

  ThreadPool &operator=(const ThreadPool &) = delete;

  ~ThreadPool();

  void submit(const Task &func);

 private:
  void workerEntry();

 private:
  std::atomic_bool done_;
  ThreadSafePriorityQueue<task_t> task_queue_;
  std::vector<std::thread> thread_pool_;
};

template <class Task>
void ThreadPool<Task>::submit(const Task &func) {
  task_queue_.push(func);
}

template <class Task>
ThreadPool<Task>::ThreadPool() : done_(false) {
  auto max_thread_num = std::thread::hardware_concurrency();
  thread_pool_.reserve(max_thread_num);
  for (; max_thread_num > 0; --max_thread_num)
    thread_pool_.push_back(
        std::thread(&ThreadPool::workerEntry, this));  // may throw
}

template <class Task>
ThreadPool<Task>::~ThreadPool() {
  // set done_ flag to true, let all thread in pool to exit
  done_ = true;
  for (auto &&t : thread_pool_) t.join();
}

template <class Task>
void ThreadPool<Task>::workerEntry() {
  for (; !done_;) {
    std::unique_ptr<task_t> task_ptr = task_queue_.pop();
    if (task_ptr != nullptr)
      (*task_ptr)();
    else
      std::this_thread::yield();
  }
}

CYZPP_END

#endif  // !_CYZPP_THREAD_POOL_H__
