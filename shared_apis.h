#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

extern int ts_open (const char* Path, int mode); 
extern int ts_close(int fd);
extern size_t ts_read (int fd, void* buf, size_t cnt);
extern size_t ts_write (int fd, void* buf, size_t cnt); 
extern int ts_ioctl (int fd, unsigned long int request, void *arg);
extern void *ts_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
extern int ts_munmap(void *addr, size_t length);
extern int ts_get_baud_rate(int fd, unsigned long int *baudrate);
extern int ts_set_baud_rate(int fd, unsigned long int *baudrate);
extern size_t ts_tx_data(int fd, void* buf, size_t cnt);
extern size_t ts_rx_data(int fd, void* buf, size_t cnt);

