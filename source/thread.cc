#include "thread.h"
#include <exception>
#include <tuple>
#include <utility>

CYZPP_BEGIN

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
  detail::thread_task_data_base *p =
      (detail::thread_task_data_base *)data;
  p->run();
  return 0;
}

// query
bool thread::joinable() const {
  return control_ != nullptr;
}

thread::handle thread::native_handle() const {
  if(control_ == nullptr)
    ::std::terminate();
  return control_->tid_;
}

// control function
int thread::start() {
  if(control_ != nullptr)
    ::std::terminate();
  control_ = ::std::make_unique<detail::thread_control_data>();
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

thread::handle thread::current_thread() noexcept {
  return pthread_self();
}

void swap(thread &lhs, thread &rhs) noexcept {
  lhs.swap(rhs);
}



CYZPP_END