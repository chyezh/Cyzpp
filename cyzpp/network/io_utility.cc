#include "io_utility.h"
#include "errno.h"

CYZPP_BEGIN

ssize_t readn(int fd, void* buff, size_t n) {
    size_t left_num = n, read_num = 0;
    char *p = static_cast<char*>(buff);
    for(;left_num > 0;) {
        if((read_num = read(fd, p, n)) < 0) {
            if(errno == EINTR)  // sig int
              read_num = 0;
            else
              return -1;
        } else if(read_num == 0) // EOF
          break;
        left_num -= read_num;
        p += read_num;
    }
    return (n - left_num);
}

ssize_t writen(int fd, void* buff, size_t n) {
    size_t left_num = n, write_num = 0;
    char *p = static_cast<char*>(buff);
    for(;left_num > 0;) {
        if((write_num = write(fd, p, n)) <= 0) {
            if(errno == EINTR)
              write_num = 0;
            else
              return -1;
        }
        left_num -= write_num;
        p += write_num;
    }
    return n;
}

CYZPP_END