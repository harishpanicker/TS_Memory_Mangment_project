#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>

int8_t write_buf[1024];
int8_t read_buf[1024];

void Set_BaudRate(int,int32_t,int32_t);
void Driver_Ack(int);

/*******************
* TODO 
*
* driver_receive();
* driver_transmit();
*
*********************/
