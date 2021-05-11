#include "shared_apis.h"
#define MAX_SIZE 1024
#define SET_BAUDRATE_VALUE _IOW('a','a',int*) 
#define GET_BAUDRATE_VALUE _IOR('a','b',int*)
#define TX_DATA _IOW('a','c',char*)
#define RX_DATA _IOR('a','d',char*)



int8_t write_buf[MAX_SIZE];
int8_t read_buf[MAX_SIZE];
unsigned long  baudrate;

int main()
{
	int fd;
	char option,choice;
	char *mapBuf;
	int number = 0, value = 0;
	fd = ts_open("/dev/My_device",O_RDWR);
	if(fd <0)
	{
		printf("can not ts_open device");
		return 0;
	}
	

	while(1)
	{
		printf("****Please Enter the Option******\n"); 
		printf("        1. Write               \n");
		printf("        2. Read                 \n");		
		printf(" 	3. IOCTL_Operations	\n");				
		printf("        4. Exit                 \n");
		printf("*********************************\n");
		scanf(" %c",&option);
		printf("Your Option = %c\n",option);
		
		switch(option)
		{
			case '1':
				printf("Enter the string to write into driver :"); 
				scanf("  %[^\t\n]s", write_buf);
				printf("Data Writing ...");
				ts_write(fd, write_buf, strlen(write_buf)+1);
				printf("Done!\n");
				break;
			case '2':
			   	printf("Data Reading ...");
			   	ts_read(fd, read_buf, MAX_SIZE);
			   	printf("Done!\n\n");
			   	printf("Data = %s\n\n", read_buf);
			  	break;
			case '3':	
				printf("perform below IOCTL operations:\n");
				printf(" 	1. Get_baudrate		\n");
				printf(" 	2. Set_baudrate		\n");
				printf(" 	3. Data transmission	\n");
				printf(" 	4. Data Reception	\n");
				printf("Enter the number perform IOCTL operations:");
				scanf(" %c",&choice);
				mapBuf = ts_mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0); //Memory mapping, will call the driver mmap function
				switch(choice)
				{
					case '1':
						printf("Getting baudrate Value from Driver...\n");
						ts_ioctl(fd, GET_BAUDRATE_VALUE, (unsigned long*)&baudrate);
						printf("Value is %ld\n",baudrate);
						break;
					case '2':
						printf("Enter the baudrate Value to set:\n");
						scanf("%ld",&baudrate);
				 		ts_ioctl(fd, SET_BAUDRATE_VALUE, (unsigned long*)&baudrate);
						break;
					case '3':
						printf("Enter the Transmiting data:"); 
						scanf("  %[^\t\n]s", mapBuf);
						printf("Data Writing ...");
						ts_ioctl(fd,TX_DATA,mapBuf);
						printf("Done!\n");
						break;		
					case '4':
			   			printf("Recevied Data...");
			   			ts_ioctl(fd,RX_DATA,mapBuf);
			   			printf("Data = %s\n\n", mapBuf);
			  			break;
					default:
				   		printf("Enter Valid optionbetween 1-4\n");
				   		break;
				}
				break;
		   	case '4':
				   printf("closing the userspace application\n");
				   ts_munmap(mapBuf, 1024); //Remove the mapping  
				   ts_close(fd);
				   exit(1);
				   break;
		   	default:
				   printf("Enter Valid option = %c\n",option);
				   break;
		}
	
	}
	
	ts_munmap(mapBuf, 1024); //Remove the mapping  
	ts_close(fd);
}
