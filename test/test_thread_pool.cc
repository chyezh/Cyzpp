#include <iostream>
#include "../cyzpp/thread/thread_pool.h"

std::mutex m;

class Task {
 public:
  Task(int num) : num_(num) {}

  ~Task() { std::cout << std::flush; }

  void operator()() {
    for (int i = 0; i < num_; ++i) {
      std::lock_guard<std::mutex> lk(m);
      std::cout << std::this_thread::get_id() << ": " << i << '\n';
    }
  }

  bool operator<(const Task& rhs) {
      return num_ < rhs.num_;
  }

  friend bool operator<(const Task &lhs, const Task &rhs);

 private:
  int num_;
};

bool operator<(const Task &lhs, const Task &rhs) { return lhs.num_ < rhs.num_; }

int main() {
  using namespace std::chrono_literals;
  cyzpp::ThreadPool<Task> thread_pool;
  for (int i = 0; i < 100; ++i) thread_pool.submit(Task{i});
  std::this_thread::sleep_for(10s);
  return 0;
}