/*4_3 Ccreate a kernel to control the GPIO pins*/

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/gpio.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

#define LED1 4
#define LED2 17
#define LED3 27
#define LED4 22

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HANG JI");
MODULE_DESCRIPTION("TIMERS");
MODULE_VERSION("0.1");

static struct hrtimer hr_timer;
static int signal = 1; 



enum hrtimer_restart hrTimerCallback(struct hrtimer *timer_for_restart) 
{
	ktime_t currtime, interval;
	currtime = ktime_get();

	if ( signal == 1)
	{
		
		interval = ktime_set(0, 1e9);
		// Move restart timer to a time in the future
		hrtimer_forward(timer_for_restart, currtime, interval);

		printk(KERN_INFO "TURN ON THE LEDs FOR 1 SECONDS\n");
	
		gpio_direction_output(LED1,1);
		gpio_direction_output(LED2,1);
		gpio_direction_output(LED3,1);
		gpio_direction_output(LED4,1);

		signal = 0;
	}
	else
	{
		interval = ktime_set(0,2e9);
		// Move restart timer to a time in the future
		hrtimer_forward(timer_for_restart, currtime, interval);

		printk(KERN_INFO "TURN OFF THE LEDs FOR 2 SECONDS\n");

		gpio_direction_output(LED1,0);
		gpio_direction_output(LED2,0);
		gpio_direction_output(LED3,0);
		gpio_direction_output(LED4,0);

		signal = 1;
	}

	// restart, or return HRTIMER_NORESTART;
	return HRTIMER_RESTART;
}



static int __init LEDS_init(void)
{	
	printk(KERN_INFO "LEDS: turn on all the LEDs...");

	//initial LEDs 

	gpio_request(LED1,"LED1");
	gpio_request(LED2,"LED1");
	gpio_request(LED3,"LED1");
	gpio_request(LED4,"LED1");

	gpio_direction_output(LED1,1);
	gpio_direction_output(LED2,1);
	gpio_direction_output(LED3,1);
	gpio_direction_output(LED4,1);


	// Initialize a hrtimer
	// a clock which is guaranteed always to move forward in time, but which does not reflect "wall clock time" in any specific way. 
	// HRTIMER_REL indicates that time should be interpreted relative to the current time
	//hrtimer_init(&hr_timer,CLOCK_MONOTONIC,HRTIMER_MODE_REL);

	ktime_t ktime; 
	printk(KERN_INFO "HR Timer module installing\n");
	ktime = ktime_set(0,1e9);
	hrtimer_init( &hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
	hr_timer.function = &hrTimerCallback;
	printk(KERN_INFO "starting timer to count 1s\n");
 	hrtimer_start( &hr_timer, ktime, HRTIMER_MODE_REL );

 
	return 0;

}

static void __exit LEDS_exit(void)
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

	return;

}

module_init(LEDS_init);
module_exit(LEDS_exit);
