/***************************************************************************
 *  \file       my_driver.c
 *
 *  \details    Linux device driver (Open,Read,Write,close,IOCTL,mmap)
 *
 *  \author     Harish,Chandan,Suresh
 *
 ****************************************************************************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>
#include <asm/io.h>			//define virt_to_phys interface  
#include <linux/mm.h>			//remap_pfn_range 

#define mem_size        1024            //Memory Size

int32_t value = 0;
uint8_t *kernel_buffer;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev dev_cdev;

/*
 ** Function Prototypes
 */
static int      __init Mydriver_init(void);
static void     __exit Mydriver_exit(void);
static int      device_open(struct inode *inode, struct file *file);
static int     	device_release(struct inode *inode, struct file *file);
static ssize_t  device_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  device_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     device_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static  int 	device_mmap( struct file *file,  struct vm_area_struct *vma);
/*
 ** File operation sturcture
 */
static struct file_operations fops =
{
	.owner          = THIS_MODULE,
	.read           = device_read,
	.write          = device_write,
	.open           = device_open,
	.mmap		= device_mmap,
	.unlocked_ioctl = device_ioctl,
	.release        = device_release,
};

/*
 ** This function will be called when we open the Device file
 */
static int device_open(struct inode *inode, struct file *file)
{
	/*Creating Physical memory*/
	if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0){
		printk(KERN_INFO "Cannot allocate memory in kernel\n");
		return -1;
	}
	pr_info("Device File Opened...!!!\n");
	return 0;
}

/*
 ** This function will be called when we close the Device file
 */
static int device_release(struct inode *inode, struct file *file)
{
	kfree(kernel_buffer);
	pr_info("Device File Closed...!!!\n");
	return 0;
}

/*
 ** This function will be called when we read the Device file
 */
static ssize_t device_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	
	pr_info("Read function called...\n");
	//kernel_buffer="Hello !!! Driver is ready";
	
	//Copy the data from the kernel space to the user-space
	if(copy_to_user(buf, kernel_buffer, mem_size))
	{
		pr_err("Data Read : Err!\n");
	}
	pr_info("Data Read : Done!\n");
	return mem_size;
}

/*
 ** This function will be called when we write the Device file
 */
static ssize_t device_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
	pr_info("Write function called...\n");
	//Copy the data to kernel space from the user-space
	if(copy_from_user(kernel_buffer, buf, len))
	{
		pr_err("Data Write : Err!\n");
	}
	//pr_info("Data Written is %s\n",kernel_buffer);
	return len;
}

/*
 ** This function will be called when we have to map memory between a device and user space
 */
static  int device_mmap( struct file *file,  struct vm_area_struct *vma)  
{

	/*TODO Implementation of mmap in Userspace and kernelspace as well*/

	pr_info("Mmap function called...!!!\n");
	return 0;
}


/*
 ** This function will be called when we write IOCTL on the Device file
 */
static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch(cmd) {
		case 9600:
			pr_info("Baud Rate = %d\n", cmd);
			value=9600;
			if( copy_to_user((int32_t*) arg, &value, sizeof(value)) )
                        {
                                pr_err("Data Read : Err!\n");
                        }
			break;
		case 115200:
			pr_info("Baud Rate = %d\n", cmd);
			value=115200;
			if( copy_to_user((int32_t*) arg, &value, sizeof(value)) )
                        {
                                pr_err("Data Read : Err!\n");
                        }
			break;
		default:
			pr_info("Default\n");
			break;
	}
	return 0;
}

/*
 ** Module Init function
 */
static int __init Mydriver_init(void)
{
	/*Allocating Major number*/
	if((alloc_chrdev_region(&dev, 0, 1, "My_Dev")) <0){
		pr_err("Cannot allocate major number\n");
		return -1;
	}
	pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

	/*Creating cdev structure*/
	cdev_init(&dev_cdev,&fops);

	/*Adding character device to the system*/
	if((cdev_add(&dev_cdev,dev,1)) < 0){
		pr_err("Cannot add the device to the system\n");
		goto r_class;
	}

	/*Creating struct class*/
	if((dev_class = class_create(THIS_MODULE,"dev_class")) == NULL){
		pr_err("Cannot create the struct class\n");
		goto r_class;
	}

	/*Creating device*/
	if((device_create(dev_class,NULL,dev,NULL,"My_device")) == NULL){
		pr_err("Cannot create the Device 1\n");
		goto r_device;
	}
	pr_info("Driver Insert Successfully...!!!\n");
	return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev,1);
	return -1;
}

/*
 ** Module exit function
 */
static void __exit Mydriver_exit(void)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&dev_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("Driver Removed Successfully...!!!\n");
}

module_init(Mydriver_init);
module_exit(Mydriver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Harish Chandan Suresh");
MODULE_DESCRIPTION("Simple Linux device driver");
MODULE_VERSION("1");
