#ifndef _CYZPP_NETWORK_SOCKET_H__
#define _CYZPP_NETWORK_SOCKET_H__

#include "../common.h"
#include <sys/socket.h>
#include "internet_address.h"

CYZPP_BEGIN

class InternetAddress;

class Socket {
 public:
  typedef int sockfd_t;
  typedef int sock_proto_t;
  typedef int sock_type_t;
  typedef int sock_domain_t;

  // constructor
  explicit Socket(sockfd_t fd) : sockfd_(fd) {}

  Socket(sock_domain_t domain, sock_type_t type, sock_proto_t protocol);

  Socket(const Socket &) = delete;

  Socket &operator=(const Socket) = delete;

  sockfd_t nativeHandler() const { return sockfd_; }

  void bind(const InternetAddress& address);
  
  void listen();

  sockfd_t accept(InternetAddress& address);

  void connect();

  void close();

 private:
  sockfd_t sockfd_;
};

CYZPP_END

#endif // _CYZPP_NETWORK_SOCKET_H__