#include "thread.h"
#include <exception>
#include <memory>
#include <tuple>
#include <utility>
#include "pthread.h"


CYZPP_BEGIN

// implement inside thread namespace
namespace multithread_using {

// data structure for packaging function to run in new thread

// control block of thread



}  // namespace multithread_using

// >>> thread manage object

// constructor


thread::thread() noexcept : task_(nullptr), control_(nullptr) {}

thread::thread(thread &&other) noexcept
    : task_(::std::move(other.task_)), control_(::std::move(other.control_)) {}


// move assignment
thread &thread::operator=(thread &&other) noexcept {
  if (control_ != nullptr) ::std::terminate();
  task_ = ::std::move(other.task_);
  control_ = ::std::move(other.control_);
  return *this;
}

// destructor
thread::~thread() {
  // terminate if "thread obejct" hold a thread
  if (control_ != nullptr) ::std::terminate();
}

// swap function
void thread::swap(thread &other) noexcept {
  ::std::swap(task_, other.task_);
  ::std::swap(control_, other.control_);
}

void *thread::thread_entry(void *data) {
  multithread_using::thread_task_data_base *p =
      (multithread_using::thread_task_data_base *)data;
  p->run();
  return 0;
}

// control function
int thread::start() {
  if(control_ != nullptr)
    ::std::terminate();
  control_ = ::std::make_unique<multithread_using::thread_control_data>();
  return pthread_create(&control_->tid_, nullptr, thread_entry, task_.get());
}

void thread::join() {
  if(control_ == nullptr)
    ::std::terminate();
  pthread_join(control_->tid_, nullptr);
  control_.release();
}

void thread::detach() {
  if(control_ == nullptr)
    ::std::terminate();
  pthread_detach(control_->tid_);
  control_.release();
}

CYZPP_END