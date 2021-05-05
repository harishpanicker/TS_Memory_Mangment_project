#include "HEADER.H"

int main()
{
	int fd;
	int32_t value, BaudRate;
	
	printf("\nOpening Device file...!!!\n");

	fd = open("/dev/My_device", O_RDWR);
	if(fd < 0) {
		printf("Cannot open device file...\n");
		return 0;
	}

	/*This function helps to get Ack from driver where we have made use of Read/Write API */
	Driver_Ack(fd);

	printf("Enter the Baud Rate to set = ");
	scanf("%d",&BaudRate);

	/*This function helps to set the baud Rate where we have made use of IOCTL API */
	Set_BaudRate(fd,BaudRate,value);


/*	TODO Make use of mmap for read/write the data from driver
*
*	void *mmap(void *addr, size_t length, int prot, int flags,
*			int fd, off_t offset);
*	int munmap(void *addr, size_t length);	
*/
	
	printf("Closing device file...!!!\n");
	close(fd);
}

void Set_BaudRate(int fd,int32_t BaudRate,int32_t value){

	switch(BaudRate){
	case 9600	: printf("Setting Baud Rate as %d\n",BaudRate);
			  ioctl(fd, BaudRate, (int32_t*) &value);
			  printf("Baud Rate set is %d\n", value); 
		   	  break;
	case 115200	: printf("Setting Baud Rate as %d\n",BaudRate);
			  ioctl(fd, BaudRate, (int32_t*) &value); 
			  printf("Baud Rate set is %d\n", value); 
		   	  break;
	default		: printf("Wrong Baud Rate..\n\n");
		   	  break;

	}


}

void Driver_Ack(int fd){

	printf("Enter the string to write into driver :");
	scanf("%[^\n]s", write_buf);
	write(fd, write_buf, strlen(write_buf)+1);
	printf("Data written !!!\n");
	sleep(3);
	read(fd, read_buf, sizeof(read_buf));
	printf("ACK from driver = %s\n", read_buf);

}
