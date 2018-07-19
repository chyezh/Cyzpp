#include "network_utility.h"
#include "errno.h"

// read and write
ssize_t readn(int fd, void* buff, size_t n) {
    ssize_t nleft = n, nread = 0;
    char* p = static_cast<char*>(buff);
    for(;nleft > 0;) {
        if((nread = read(fd, p, nleft)) < 0) {
            if(errno == EINTR) // recall read
              nread = 0;
            else
              return -1;
        } else if(nread == 0) // touch EOF
          break;
        nleft -= nread;
        p += nread;
    }
    return n - nleft;
}

ssize_t wirten(int fd, void* buff, size_t n) {
    ssize_t nleft = n, nwrite = 0;
    char * p = static_cast<char*>(buff);
    for(;nleft > 0;) {
        if((nwrite = write(fd, buff, nleft)) < 0) {
            if(errno == EINTR)
              nwrite = 0;
            else
              return -1;
        } else if(nwrite == 0) {
            return -1;
        }
        nleft -= nwrite;
        p += nwrite;
    }
    return n;
}

ssize_t readline(int fd, void* buff, size_t n);




