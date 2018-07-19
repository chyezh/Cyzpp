#ifndef _CYZPP_NETWORK_UTILITY_H__
#define _CYZPP_NETWORK_UTILITY_H__

#include <unistd.h>
#include <arpa/inet.h>

// address transform

// read and write
ssize_t readn(int fd, void* buff, size_t n);

ssize_t wirten(int fd, void* buff, size_t n);

ssize_t readline(int fd, void* buff, size_t n);


#endif // !_CYZPP_NETWORK_UTILITY_H__
