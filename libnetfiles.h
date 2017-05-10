#ifndef _HEADER_H
#define _HEADER_H

int netserverinit(char * hostname,int mode);
int netopen(const char * pathname, int flags);
ssize_t netread(int fildes, void *buff, size_t nbyte);
ssize_t netwrite(int fildes, const void *buff, size_t nbyte);
int netclose (int fd);

#endif