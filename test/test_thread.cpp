#include "../source/thread.h"
#include <iostream>

void print(int& i, int k) {
    i = k;
}

int main() {
    int a1 = 0, a2 = 0;
    cyzpp::thread t1(print, a1, 1);
    cyzpp::thread t2(print, a2, 2);
    t1.start();
    t2.start();
    t1.join();
    if(t1.joinable())
      std::cout << "t1 joinable, ";
    if(t2.joinable())
      std::cout << "t2 joinable\n";
    swap(t1, t2);
    if(t1.joinable())
      std::cout << "t1 joinable, ";
    if(t2.joinable())
      std::cout << "t2 joinable\n";
    t1.detach();
    std::cout << a1 << ' ' << a2 << '\n';
    return 0;
}

