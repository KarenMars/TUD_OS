#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>
#include <unistd.h>

#define LED1 7
#define LED2 0
#define LED3 2
#define LED4 3


pthread_barrier_t our_barrier;

void *Ulit2Lit(void *param);
void *Lit2Ulit(void *param);

int main(void)
{
	wiringPiSetup();
	/*initialise pin for PWN output*/
	pinMode(LED1,OUTPUT);
	pinMode(LED2,OUTPUT);
	pinMode(LED3,OUTPUT);
	pinMode(LED4,OUTPUT);

	softPwmCreate(LED1,0,100);
	softPwmCreate(LED2,0,100);
	softPwmCreate(LED3,0,100);
	softPwmCreate(LED4,0,100);



	/*
	digitalWrite(LED1,HIGH);
	digitalWrite(LED2,HIGH);
	digitalWrite(LED3,HIGH);
	digitalWrite(LED4,HIGH);
	*/

	pthread_barrier_init(&our_barrier,NULL,2);

	/*create threads*/
	pthread_t tid1;
	pthread_t tid2;

	pthread_create(&tid1,NULL,Ulit2Lit,NULL);
	pthread_create(&tid2,NULL,Lit2Ulit,NULL);
	

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	pthread_barrier_destroy(&our_barrier);
	
}


void *Ulit2Lit(void *param)
{
	int i=0;
	printf("threads 1: ulit to lit\n");


	for (i=0;i<=100;i++)
	{
		
		softPwmWrite(LED1,i);
		softPwmWrite(LED2,i);
		softPwmWrite(LED3,i);
		softPwmWrite(LED4,i);
		usleep(100000);
	}
	printf("threads 1: finish\n");

	pthread_barrier_wait(&our_barrier);

	return 0;

}


void *Lit2Ulit(void *param)
{
	/*wait for thread 1 to finish*/

	
	pthread_barrier_wait(&our_barrier);

	int i=100;

	printf("threads 2: lit to unlit\n");


	for (i=100;i>=0;i--)
	{
		softPwmWrite(LED1,i);
		softPwmWrite(LED2,i);
		softPwmWrite(LED3,i);
		softPwmWrite(LED4,i);
		usleep(100000);
	}
	
	printf("threads 2: finish\n");

	return 0;
}