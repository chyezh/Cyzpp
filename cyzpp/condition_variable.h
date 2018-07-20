#ifndef _CYZPP_CONDITION_VARIABLE_H__
#define _CYZPP_CONDITION_VARIABLE_H__
#include "common.h"
#include "mutex.h"

CYZPP_BEGIN

class condition_variable {
 public:
  typedef pthread_cond_t handle;

  condition_variable() : cond_(PTHREAD_COND_INITIALIZER), p_mutex_(nullptr) {}

  condition_variable(const condition_variable &) = delete;

  condition_variable &operator=(const condition_variable &) = delete;

  ~condition_variable() {
      pthread_cond_destroy(&cond_);
  }

  handle* native_handle() { return &cond_; }

  void notify_all() noexcept {

  }

  void notify_one() noexcept {

  }


 private:
  handle cond_;
  mutex* p_mutex_;
};


CYZPP_END



#endif // !_CYZPP_CONDITION_VARIABLE_H__
