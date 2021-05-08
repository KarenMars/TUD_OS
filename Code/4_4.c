/*4_4: a simple device driver*/
/*pwm*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

/*convert char to int*/
#include <linux/debugfs.h>
#include <uapi/linux/stat.h> 

#define LED1 4 
#define LED2 17
#define LED3 27
#define LED4 22



#define DEVICE_NAME "pwm"
#define CLASS_NAME "pwm_file"
#define SUCCESS 0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HANG JI");
MODULE_DESCRIPTION("PWM");
MODULE_VERSION("0.1");

static int Major; // the device numner
static struct class* clp = NULL; //the device-driver class struct pointer
static struct device* dvp = NULL; // the device-driver device pointer

static struct mutex myMutex;
static int fopen = 0; // whether the device is open or not



//the operation functions for the character device 
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_write (struct file *, const char *, size_t, loff_t *);

static unsigned long long PWM_INPUT;

static struct file_operations fops =
{
	.open = dev_open,
	.release = dev_release,
	.write = dev_write,
};

//hr_timer 

static struct hrtimer hr_timer;
static int signal = 1; 


enum hrtimer_restart hrTimerCallback(struct hrtimer *timer_for_restart) 
{
	ktime_t currtime, interval;
	currtime = ktime_get();

	if ( signal == 1)
	{
		
		interval = ktime_set(0,PWM_INPUT*100000);
		// Move restart timer to a time in the future
		hrtimer_forward(timer_for_restart, currtime, interval);

		//printk(KERN_INFO "PWM HIGH VALUE\n");
	
		gpio_direction_output(LED1,1);
		gpio_direction_output(LED2,1);
		gpio_direction_output(LED3,1);
		gpio_direction_output(LED4,1);

		signal = 0;

	}
	else
	{
		interval = ktime_set(0,10000000-PWM_INPUT*100000);
		// Move restart timer to a time in the future
		hrtimer_forward(timer_for_restart, currtime, interval);

		//printk(KERN_INFO "PWM LOW VALUE\n");

		gpio_direction_output(LED1,0);
		gpio_direction_output(LED2,0);
		gpio_direction_output(LED3,0);
		gpio_direction_output(LED4,0);

		signal = 1;
	}

	// restart, or return HRTIMER_NORESTART;
	return HRTIMER_RESTART;
}


static int __init chardev_init(void)
{

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

	dvp = device_create(clp, NULL, MKDEV(Major,0),NULL, DEVICE_NAME);

	printk(KERN_INFO "device created successfully\n");


	gpio_request(LED1,"LED1");
	gpio_request(LED2,"LED1");
	gpio_request(LED3,"LED1");
	gpio_request(LED4,"LED1");

	// gpio_direction_output(LED1,1);
	// gpio_direction_output(LED2,1);
	// gpio_direction_output(LED3,1);
	// gpio_direction_output(LED4,1);

	printk(KERN_INFO "leds initialized successfully\n");

	ktime_t ktime;
	printk(KERN_INFO "HR Timer module installing\n");
	ktime = ktime_set(0,1e6);
	hrtimer_init( &hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
	hr_timer.function = &hrTimerCallback;
	printk(KERN_INFO "starting timer to dimming LEDs using PWM\n");
 	hrtimer_start( &hr_timer, ktime, HRTIMER_MODE_REL );

	return SUCCESS;

}

static void __exit chardev_exit(void)
{
	printk(KERN_INFO "free the used GPIO pins");

	gpio_free(LED1);
	gpio_free(LED2);
	gpio_free(LED3);
	gpio_free(LED4);

	int ret;
  	ret = hrtimer_cancel( &hr_timer );

  	if (ret) 
  	{
  		printk("The timer was still in use...\n");
  	}

  	printk("HR Timer module uninstalling\n");

	printk(KERN_INFO "Cancel timer successfully");

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


static ssize_t dev_write(struct file *filp, const char *buff, size_t length, loff_t * offset)
{	

	mutex_lock(&myMutex);

  	int retval = 0 ;

    retval = kstrtoull_from_user(buff, length, 10, &PWM_INPUT);

    if ( retval || (PWM_INPUT>100 || PWM_INPUT<0)) 
    {
    	PWM_INPUT = 0; 
		// gpio_direction_output(LED1,0);
		// gpio_direction_output(LED2,0);
		// gpio_direction_output(LED3,0);
		// gpio_direction_output(LED4,0);
        printk(KERN_INFO "Negative error code\n");
        mutex_unlock(&myMutex);
        return -EFAULT; 
    } 
    	
    else 
    {
        printk(KERN_INFO "The input value of PWM is %llu\n", PWM_INPUT);
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









