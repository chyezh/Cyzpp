#ifndef _CYZPP_THREAD_H__
#define _CYZPP_THREAD_H__
#include <memory>
#include <functional>
#include "common.h"
#include "utility.h"

CYZPP_BEGIN
namespace multithread_using {

struct thread_task_data_base {
    virtual ~thread_task_data_base() {}
    virtual void run() = 0;
};

struct thread_control_data {
  thread_control_data() : tid_(0) {}

  thread_control_data(const thread_control_data&) = delete;

  thread_control_data &operator=(const thread_control_data&) = delete;

  pthread_t tid_;
};

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
  void in_run(compile_time_using::sequence_index<Index...>) {
     ::std::invoke(::std::get<0>(tp_),
           std::get<Index>(tp_)...);
  }

  void run() {
    // run the f(args...)
    typedef typename compile_time_using::make_sequence_index<
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

  // thread operation
  int start();

  void join();

  void detach();

 public:
  static void *thread_entry(void *);

  ::std::unique_ptr<multithread_using::thread_task_data_base> task_;
  ::std::unique_ptr<multithread_using::thread_control_data> control_;
};

template <class Function, class... Args>
thread::thread(Function &&f, Args &&... args)
    : task_(multithread_using::make_thread_task_data(::std::forward<Function>(f),
                                  ::std::forward<Args>(args)...)),
      control_(nullptr) {}

class mutex {};

class condition_variable {};

CYZPP_END

#endif  // _CYZPP_THREAD_H__