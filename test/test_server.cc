#include <unistd.h>
#include <cstdio>
#include <cstring>
#include "socket.h"

using namespace std;

int main() {
  sockaddr_in a;
  bzero(&a, sizeof(a));
  a.sin_family = AF_INET;
  a.sin_addr.s_addr = htonl(INADDR_ANY);
  a.sin_port = htons(13);
  cyzpp::InternetAddress addr(a);
  cyzpp::Socket sock(AF_INET, SOCK_STREAM, 0);
  sock.bind(addr);
  sock.listen();
  for (;;) {
    int sock2 = sock.accept(addr);
    if (fork() == 0) {
      sock.close();
      std::printf("connect from %s\n", addr.address().c_str());
      time_t ticks = time(NULL);
      char buff[4096];
      snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
      write(sock2, buff, strlen(buff));
      exit(0);
    }
    close(sock2);
  }
  sock.close();
}
