#ifndef _CYZPP_THREADS_H__
#define _CYZPP_THREADS_H__
#include "common.h"

CYZPP_BEGIN

namespace multithread_using {
    struct thread_data;
} // namespace multithread_using

// >>> thread manage object

class thread {
 public:
  // constructor
  thread() noexcept;

  thread(const thread &) = delete;
  
  thread(thread &&) noexcept;

  ~thread();
  // assignment
  thread &operator=(const thread &) = delete;

  thread &operator=(thread &&) noexcept;

  void swap(thread &) noexcept;

  // thread operation
  void join();

  void detach();

 private:
  multithread_using::thread_data *data_;
};

class mutex {};

class condition_variable {};

CYZPP_END

#endif  // _CYZPP_THREADS_H__