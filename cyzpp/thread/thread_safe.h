#ifndef _CYZPP_THREAD_SAFE_H__
#define _CYZPP_THREAD_SAFE_H__
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include "../basic/common.h"

// thread safe task queue
template <class T, class Allocator = std::allocator<T>>
class ThreadSafeQueue {
 public:
  // >>> member type
  typedef std::deque<T, Allocator> container_type;
  typedef typename container_type::size_type value_type;
  typedef typename container_type::size_type iterator;
  typedef typename container_type::size_type const_iterator;
  typedef typename container_type::reference reference;
  typedef typename container_type::const_reference const_reference;

 public:
  // constructor
  ThreadSafeQueue() {}

  ThreadSafeQueue(const ThreadSafeQueue &queue) = delete;

  ThreadSafeQueue &operator=(const ThreadSafeQueue &) = delete;

  bool empty() const;

  void push(value_type value);

  std::unique_ptr<value_type> pop();

  // wait until queue could pop an element 
  std::unique_ptr<value_type> wait_pop();

 private:
  std::deque<T, Allocator> container_;
  mutable std::mutex mutex_;
  mutable std::condition_variable cond_;
};

template <class T, class Allocator>
bool ThreadSafeQueue<T, Allocator>::empty() const {
  std::lock_guard<std::mutex> lk(mutex_);
  return container_.empty();
}

template <class T, class Allocator>
void ThreadSafeQueue<T, Allocator>::push(value_type value) {
  std::lock_guard<std::mutex> lk(mutex_);
  container_.emplace_back(value);
  cond_.notify_one();
}

template <class T, class Allocator>
std::unique_ptr<typename ThreadSafeQueue<T, Allocator>::value_type>
ThreadSafeQueue<T, Allocator>::pop() {
  std::lock_guard<std::mutex> lk(mutex_);
  if (container_.empty())
    // return null pointer if empty
    return nullptr;
  else {
    std::unique_ptr<T, Allocator> p = std::make_unique<T>(container_.front());
    container_.pop_front();
    return p;
  }
}

template <class T, class Allocator>
std::unique_ptr<typename ThreadSafeQueue<T, Allocator>::value_type>
ThreadSafeQueue<T, Allocator>::wait_pop() {
  std::unique_lock<std::mutex> lk(mutex_);
  // check empty.
  cond_.wait(lk, [this](){ return !container_.empty(); });
  std::unique_ptr<T, Allocator> p = std::make_unique<T>(container_.front());
  container_.pop_front();
  return p;
}

#endif  // !_CYZPP_THREAD_SAFE_H__
