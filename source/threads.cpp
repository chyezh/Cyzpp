#include "threads.h"
#include "pthread.h"
#include <exception>

CYZPP_BEGIN

namespace multithread_using {

struct thread_data {
  pthread_t tid_;
};

}  // namespace multithread_using

// >>> thread manage object

// constructor
thread::thread() noexcept : data_(nullptr) {}

thread::thread(thread &&other) noexcept : data_(other.data_) {
  other.data_ = nullptr;
}



// move assignment
thread &thread::operator=(thread && other) noexcept {
  if (data_ != nullptr)
    ::std::terminate();
  data_ = other.data_;
  other.data_ = nullptr;
  return *this;
}

thread::~thread() {
  // do processing if "thread obejct" hold a thread
  if (data_ != nullptr) {
  }
}

CYZPP_END