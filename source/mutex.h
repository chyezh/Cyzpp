#ifndef _CYZPP_MUTEX_H__
#define _CYZPP_MUTEX_H__
#include "common.h"
#include "utility.h"
#include "thread.h"
#include <cassert>

CYZPP_BEGIN

class mutex {
 public:
  typedef pthread_mutex_t handle;
  typedef pthread_t holder;
  mutex() : mutex_(PTHREAD_MUTEX_INITIALIZER), tid_(0) {}

  mutex(const mutex &) = delete;

  mutex &operator=(const mutex &) = delete;

  ~mutex() {
    assert(tid_ == 0);
    pthread_mutex_destroy(&mutex_);
  }

  handle* native_handle() { return &mutex_; }

  void lock() {
    assert(pthread_mutex_lock(&mutex_) == 0);
    tid_ = thread::current_thread();
  }

  bool try_lock() {
    if(pthread_mutex_trylock(&mutex_) == 0) {
      tid_ = thread::current_thread();
      return true;
    }
    return false;
  }

  void unlock() {
    tid_ = 0;
    assert(pthread_mutex_unlock(&mutex_) == 0);
  }

 private:
   handle mutex_;
   holder tid_;
};

class timed_mutex {
 public:
  typedef pthread_mutex_t handle;
  typedef pthread_t holder;
  timed_mutex() : mutex_(PTHREAD_MUTEX_INITIALIZER), tid_(0) {}

  timed_mutex(const mutex &) = delete;

  timed_mutex &operator=(const timed_mutex &) = delete;

  ~timed_mutex() {
    assert(tid_ == 0);
    pthread_mutex_destroy(&mutex_);
  }

  handle* native_handle() { return &mutex_; }

  void lock() {
    assert(pthread_mutex_lock(&mutex_) == 0);
    tid_ = thread::current_thread();
  }

  bool try_lock() {
    if(pthread_mutex_trylock(&mutex_) == 0) {
      tid_ = thread::current_thread();
      return true;
    }
    return false;
  }

  void try_lock_for() {

  }

  void try_lock_until() {
    
  }

  void unlock() {
    tid_ = 0;
    assert(pthread_mutex_unlock(&mutex_) == 0);
  }

 private:
   handle mutex_;
   holder tid_;
};


CYZPP_END

#endif