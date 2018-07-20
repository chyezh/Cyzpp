#ifndef _CYZPP_IO_UTILITY_H__
#define _CYZPP_IO_UTILITY_H__

#include "../common.h"
#include <unistd.h>

CYZPP_BEGIN

// block_io read n bytes into buff
ssize_t readn(int fd, void* buff, size_t n);

// wirten n bytes in buff
ssize_t writen(int fd, void* buff, size_t n);

CYZPP_END

#endif // !_CYZPP_IO_UTILITY_H__