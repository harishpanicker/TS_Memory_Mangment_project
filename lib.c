#include "shared_apis.h"

#define SIZE_BAUDRATE 8

int ts_open (const char* Path,int mode)
{
	return open(Path,mode);
}

int ts_close(int fd)
{
	return close(fd);
}

size_t ts_read (int fd, void* buf, size_t cnt)
{
	return read(fd,buf,cnt);
}

size_t ts_write (int fd, void* buf, size_t cnt)
{
	return write(fd,buf,cnt);
}

int ts_ioctl (int fd, unsigned long int request,void *arg)
{
	return ioctl(fd,request,arg);
}

void *ts_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	return mmap(addr,length,prot,flags,fd,offset);
}

int ts_munmap(void *addr, size_t length)
{
	return munmap(addr,length);
}


int ts_get_baud_rate(int fd, unsigned long int *baudrate)
{
	
	return read(fd,baudrate,SIZE_BAUDRATE);
}

int ts_set_baud_rate(int fd, unsigned long int *baudrate)
{
	
	return write(fd,baudrate,SIZE_BAUDRATE);
}

size_t ts_tx_data(int fd, void* buf, size_t cnt)
{
	return write(fd,buf,cnt);
}

size_t ts_rx_data(int fd, void* buf, size_t cnt)
{
	return read(fd,buf,cnt);
}



       
