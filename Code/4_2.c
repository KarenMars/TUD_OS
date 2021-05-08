/*4_2: a simple device driver*/
/*accepts string input and prints string input in the kernel log*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "klog"
#define CLASS_NAME "klog_file"
#define SUCCESS 0

#define MAX_LENGTH 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HANG JI");
MODULE_DESCRIPTION("A SIMPLE CHARACTER DEVICE DRIVER");
MODULE_VERSION("0.2");

static int Major; // the device numner
static struct class* clp = NULL; //the device-driver class struct pointer
//static struct device* dvp = NULL; // the device-driver device pointer

static struct mutex myMutex;
static int fopen = 0; // whether the device is open or not

static char data[1024] = {0}; 
//static size_t data_len;

// static char message[256] = {0};
// static short size_of_message;

//the operation functions for the character device 
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_write (struct file *, const char *, size_t, loff_t *);


static struct file_operations fops =
{
	.open = dev_open,
	.release = dev_release,
	.write = dev_write,
};


static int __init chardev_init(void)
{
	/*
	int register_chrdev(unsigned int <major number>,
	const char * <device name>,
	const struct file_operations* <file operations callbacks>)
	*/

	Major = register_chrdev(0,DEVICE_NAME,&fops);

	
	/*Major is the number assigned to the device driver*/
	if ( Major < 0)
	{
		printk(KERN_ALERT "failed to register the device driver with %d\n", Major);
		return Major;
	}

	mutex_init(&myMutex);

	printk(KERN_INFO "register_chardev:The device driver is assigned to %d\n", Major);

	/*create a device class*/

	clp = class_create(THIS_MODULE, CLASS_NAME);

	device_create(clp, NULL, MKDEV(Major,0),NULL, DEVICE_NAME);

	return SUCCESS;

}

static void __exit chardev_exit(void)
{

	mutex_destroy(&myMutex);
	// remove the device
	device_destroy(clp, MKDEV(Major, 0));
	// unregister the device class
	class_unregister(clp);
	// remove the device class
	class_destroy(clp);
	// unregister the major number
	unregister_chrdev(Major, DEVICE_NAME);

	printk(KERN_INFO "chardev_exit: The device driver %d is destroyed\n",Major);
}

module_init(chardev_init);
module_exit(chardev_exit);

//file operation functions

static int dev_open(struct inode *inode, struct file *file)
{	

	if ( fopen == 0)
	{
		printk(KERN_INFO "dev_open: The file with major number %d has been opened\n", Major);
		fopen = 1;
		return 0;
	}
	else
	{
		printk(KERN_INFO "dev_open: The file is already opened & return error\n");
		return -EBUSY;
	}
	// printk(KERN_INFO "dev_open: The file with major number %d has been opened\n", Major);
	// fopen = 1;
	return 0;
}

//??????????????????????????????????????????????????
//buffer to fill with data; length of the buffer; 
static ssize_t dev_write(struct file *filp, const char *buff, size_t length, loff_t * offset)
{	

	// ssize_t retval;

	// mutex_lock(&myMutex);

	// data = kmalloc(length,GFP_KERNEL);

	// if (!data)
	// {
	// 	retval = -ENOMEM;
	// 	goto out;
	// }
	// /*copy_from_user: return zero on success and non-zero to indicate a failure */
	// if ( copy_from_user(data,buff,length)!=0 )
	// {	
	// 	kfree(data);
	// 	retval = -EFAULT;
	// 	goto out; 
	// }

	// printk(KERN_INFO "print user input%s\n", data);

	// kfree(data);

	// retval = length;

	// out:
	// 	mutex_unlock(&myMutex);
	// 	return retval; 

 	//   return length;

	mutex_lock(&myMutex);
 	if (length>=MAX_LENGTH)
 	{
 		length = MAX_LENGTH;
 	}
 	if ( copy_from_user(data, buff, length+1)!=0)
 	{
 		printk(KERN_INFO "failed to receive data from user\n");
 		mutex_unlock(&myMutex);
 		return -EFAULT;
 	}
 	else
 	{
 		printk(KERN_INFO "user input data is %s\n", data);
 		mutex_unlock(&myMutex);
 		return length; 
 	}


}

static int dev_release(struct inode *inodeP, struct file *fileP)
{	
	fopen = 0;
	printk(KERN_INFO "Device successfully released\n");
	return 0;
}








