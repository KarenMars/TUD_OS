/*4_1.Ccreate a kernel to control the GPIO pins*/

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/gpio.h>


#define LED1 4
#define LED2 17
#define LED3 27
#define LED4 22

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HANG JI");
MODULE_DESCRIPTION("CONTROL GPIO");
MODULE_VERSION("0.1");


static int __init leds_init(void)
{	
	printk(KERN_INFO "turn on all the LEDs...");

	gpio_request(LED1,"LED1");
	gpio_request(LED2,"LED1");
	gpio_request(LED3,"LED1");
	gpio_request(LED4,"LED1");

	gpio_direction_output(LED1,1);
	gpio_direction_output(LED2,1);
	gpio_direction_output(LED3,1);
	gpio_direction_output(LED4,1);

	printk(KERN_INFO "...turn on all the leds done\n");

	return 0;

}

static void __exit leds_exit(void)
{
	printk(KERN_INFO "turn off all the LEDs...");

	gpio_free(LED1);
	gpio_free(LED2);
	gpio_free(LED3);
	gpio_free(LED4);

	printk(KERN_INFO "...turn off all the LEDs done\n");

}

module_init(leds_init);
module_exit(leds_exit);
