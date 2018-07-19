#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include "internet_address.h"

int main() {
  sockaddr_in p;
  bzero(&p, sizeof(sockaddr_in));
  p.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &p.sin_addr);
  p.sin_port = htons(43990);
  cyzpp::InternetAddress ad(p);
  std::cout << ad.address() << '\n';
  sockaddr_in6 pp;
  bzero(&pp, sizeof(sockaddr_in6));
  pp.sin6_family = AF_INET6;
  inet_pton(AF_INET6, "ABCD:EF01:2345:6789:ABCD:EF01:2345:6789", &pp.sin6_addr);
  pp.sin6_port = htons(65500);
  cyzpp::InternetAddress ad2(pp);
  std::cout << ad2.address() << "\n";
}