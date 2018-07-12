#include "../source/threads.h"
#include <iostream>

void print(int i, int k) {
    i = k;
}

int main() {
    int a1 = 0, a2 = 0;
    cyzpp::thread t1(print, std::ref(a1), 1);
    cyzpp::thread t2(print, std::ref(a2), 2);
    t1.start();
    t2.start();
    t1.join();
    t2.join();
    std::cout << a1 << ' ' << a2 << '\n';
    return 0;
}

