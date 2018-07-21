#ifndef _CYZPP_THREAD_SAFE_H__
#define _CYZPP_THREAD_SAFE_H__
#include <condition_variable>
#include <queue>
#include <memory>
#include <mutex>
#include "../basic/common.h"

// thread safe task queue
template <class T>
class ThreadSafeQueue {
 public:
  // >>> member type
  typedef std::queue<T> container_type;
  typedef typename container_type::value_type value_type;

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
  std::queue<T> container_;
  mutable std::mutex mutex_;
  mutable std::condition_variable cond_;
};

template <class T>
bool ThreadSafeQueue<T>::empty() const {
  std::lock_guard<std::mutex> lk(mutex_);
  return container_.empty();
}

template <class T>
void ThreadSafeQueue<T>::push(value_type value) {
  std::lock_guard<std::mutex> lk(mutex_);
  container_.emplace(value);
  cond_.notify_one();
}

template <class T>
std::unique_ptr<typename ThreadSafeQueue<T>::value_type>
ThreadSafeQueue<T>::pop() {
  std::lock_guard<std::mutex> lk(mutex_);
  if (container_.empty())
    // return null pointer if empty
    return nullptr;
  else {
    std::unique_ptr<T> p = std::make_unique<T>(container_.front());
    container_.pop();
    return p;
  }
}

template <class T>
std::unique_ptr<typename ThreadSafeQueue<T>::value_type>
ThreadSafeQueue<T>::wait_pop() {
  std::unique_lock<std::mutex> lk(mutex_);
  // check empty.
  cond_.wait(lk, [this](){ return !container_.empty(); });
  std::unique_ptr<T> p = std::make_unique<T>(container_.front());
  container_.pop();
  return p;
}

// thread safe task priority queue
template <class T>
class ThreadSafePriorityQueue {
 public:
  // >>> member type
  typedef std::priority_queue<T> container_type;
  typedef typename container_type::value_type value_type;

 public:
  // constructor
  ThreadSafePriorityQueue() {}

  ThreadSafePriorityQueue(const ThreadSafePriorityQueue &queue) = delete;

  ThreadSafePriorityQueue &operator=(const ThreadSafePriorityQueue &) = delete;

  bool empty() const;

  void push(value_type value);

  std::unique_ptr<value_type> pop();

  // wait until queue could pop an element 
  std::unique_ptr<value_type> wait_pop();

 private:
  std::priority_queue<T> container_;
  mutable std::mutex mutex_;
  mutable std::condition_variable cond_;
};

template <class T>
bool ThreadSafePriorityQueue<T>::empty() const {
  std::lock_guard<std::mutex> lk(mutex_);
  return container_.empty();
}

template <class T>
void ThreadSafePriorityQueue<T>::push(value_type value) {
  std::lock_guard<std::mutex> lk(mutex_);
  container_.emplace(value);
  cond_.notify_one();
}

template <class T>
std::unique_ptr<typename ThreadSafePriorityQueue<T>::value_type>
ThreadSafePriorityQueue<T>::pop() {
  std::lock_guard<std::mutex> lk(mutex_);
  if (container_.empty())
    // return null pointer if empty
    return nullptr;
  else {
    std::unique_ptr<T> p = std::make_unique<T>(container_.top());
    container_.pop();
    return p;
  }
}

template <class T>
std::unique_ptr<typename ThreadSafePriorityQueue<T>::value_type>
ThreadSafePriorityQueue<T>::wait_pop() {
  std::unique_lock<std::mutex> lk(mutex_);
  // check empty.
  cond_.wait(lk, [this](){ return !container_.empty(); });
  std::unique_ptr<T> p = std::make_unique<T>(container_.top());
  container_.pop();
  return p;
}

#endif  // !_CYZPP_THREAD_SAFE_H__
