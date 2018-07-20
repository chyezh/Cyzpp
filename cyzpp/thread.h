#ifndef _CYZPP_THREAD_H__
#define _CYZPP_THREAD_H__
#include <memory>
#include <functional>
#include "common.h"
#include "utility.h"
#include "pthread.h"

CYZPP_BEGIN
namespace detail {

// data structure for packaging function to run in new thread
struct thread_task_data_base {
    virtual ~thread_task_data_base() {}
    virtual void run() = 0;
};

// control block of thread
struct thread_control_data {
  typedef pthread_t handle;

  thread_control_data() : tid_(0) {}

  thread_control_data(const thread_control_data&) = delete;

  thread_control_data &operator=(const thread_control_data&) = delete;

  handle tid_;
};

// data structure for packaging function to run in new thread
template <class Function, class... Args>
struct thread_task_data : public thread_task_data_base {
 public:
  thread_task_data() = delete;

  thread_task_data(const thread_task_data &) = delete;

  thread_task_data &operator=(const thread_task_data &) = delete;

  thread_task_data(Function &&f, Args&&... args)
      : tp_(::std::forward<Function>(f), ::std::forward<Args>(args)...) {}

  // package the f(args...
  template <size_t... Index>
  void in_run(utility::sequence_index<Index...>) {
     ::std::get<0>(tp_)(std::get<Index>(tp_)...);
  }

  void run() {
    // run the f(args...)
    typedef typename utility::make_sequence_index<
        1, ::std::tuple_size<std::tuple<Function, Args...>>::value - 1>::type
        index_type;
    in_run(index_type{});
  }

 private:
  ::std::tuple<typename ::std::decay<Function>::type,
               typename ::std::decay<Args>::type...>
      tp_;
};

// factory function for thread proc data
template <typename Function, class... Args>
inline ::std::unique_ptr<thread_task_data_base> make_thread_task_data(
    Function &&f, Args &&... args) {
  return ::std::make_unique<
      thread_task_data<typename std::remove_reference<Function>::type, Args...>>(
          std::forward<Function>(f), std::forward<Args>(args)...);
}

}  // namespace multithread_using

// >>> thread manage object

class thread {
 public:
  // member type
  typedef typename detail::thread_control_data::handle handle;

  // constructor
  thread() noexcept;

  thread(const thread &) = delete;

  thread(thread &&) noexcept;

  template <class Function, class... Args>
  thread(Function &&, Args &&...);

  ~thread();
  // assignment
  thread &operator=(const thread &) = delete;

  thread &operator=(thread &&) noexcept;

  void swap(thread &) noexcept;

  // query
  bool joinable() const;

  handle native_handle() const;

  // thread operation
  int start();

  void join();

  void detach();

  static handle current_thread() noexcept;

 private:
  static void *thread_entry(void *);

  ::std::unique_ptr<detail::thread_task_data_base> task_;
  ::std::unique_ptr<detail::thread_control_data> control_;
};

template <class Function, class... Args>
thread::thread(Function &&f, Args &&... args)
    : task_(detail::make_thread_task_data(::std::forward<Function>(f),
                                  ::std::forward<Args>(args)...)),
      control_(nullptr) {}

void swap(thread &lhs, thread &rhs) noexcept;

CYZPP_END

#endif  // _CYZPP_THREAD_H__