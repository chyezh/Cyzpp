#include "socket.h"
#include <unistd.h>

CYZPP_BEGIN

class InternetAddress;

Socket::Socket(sock_domain_t domain, sock_type_t type, sock_proto_t protocol) {
  sockfd_ = socket(domain, type, protocol);
}

Socket::Socket(Socket &&sock) : sockfd_(std::move(sock.sockfd_)) {
  sockfd_ = -1;
}

Socket &Socket::operator=(Socket &&sock) {
  if (this != &sock) {
    sockfd_ = std::move(sock.sockfd_);
    sock.sockfd_ = -1;
  }
  return *this;
}

void Socket::bind(const InternetAddress &address) {
  int bind_result = 0;
  switch (address.family()) {
    case AF_INET:
      bind_result =
          ::bind(sockfd_, address.getNativeAddress(), sizeof(sockaddr_in));
      break;
    case AF_INET6:
      bind_result =
          ::bind(sockfd_, address.getNativeAddress(), sizeof(sockaddr_in6));
      break;
  }
  // error handle
  if (bind_result < 0) perror("Socket::bind#");
}

void Socket::listen() {
  if (::listen(sockfd_, SOMAXCONN) < 0) perror("Socket:listen#");
}

Socket Socket::accept(InternetAddress &address) {
  socklen_t len = sizeof(InternetAddress::storage_t);
  sockfd_t accept_result = -1;
  for (;;) {
    accept_result = ::accept(
        sockfd_, const_cast<struct sockaddr *>(address.getNativeAddress()),
        &len);
    // error handler
    if (accept_result == -1) {
      // restart accpet when EINTR or ECONNABORTED
      if (errno == EINTR || errno == ECONNABORTED)
        continue;
      else
        perror("Socket::accept#");
    } else
      break;
  }
  if (accept_result < 0) perror("Socket::accept#");
  return Socket(accept_result);
}

bool Socket::connect(const InternetAddress &address) {
  return ::connect(sockfd_, address.getNativeAddress(),
                   sizeof(sockaddr_storage)) == 0;
}

void Socket::close() {
  if (::close(sockfd_) < 0) perror("Socket::close#");
}

CYZPP_END
