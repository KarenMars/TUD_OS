#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <pthread.h>
#include <unistd.h>


#define LED1 7
#define LED2 0
#define LED3 2
#define LED4 3


int LightNumber=0;

void *counter(void *param);
void *input(void *param);

void light(int a);

int main(void)
{

	/*initialize lights*/
	wiringPiSetup();

	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);

	digitalWrite(LED4, HIGH);
	digitalWrite(LED3, HIGH);
	digitalWrite(LED2, HIGH);
	digitalWrite(LED1, HIGH);

	pthread_t tid1;
	pthread_t tid2;

	usleep(2000000);
	pthread_create(&tid1, NULL, input, NULL);
	pthread_create(&tid2, NULL, counter, NULL);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	
	return 0;
}

void *input(void *param)
{	

	while (1)
	{
		/*printf("thread of input\n");*/

		printf("give a number between 0 to 15 to set the status of the led light\n");
		scanf("%d",&LightNumber);
		if ( (LightNumber<0 || LightNumber>15) )
		printf("Out of Range!!! Please give a right number\n");
		if( LightNumber == -1)
		{
			break;
		}

	}
	return 0;

}

void *counter(void *param)
{	
	wiringPiSetup();

	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);

	int counter = 0;

	/*initialize leds*/
	digitalWrite(LED4,HIGH);
	digitalWrite(LED3,HIGH);
	digitalWrite(LED2,HIGH);
	digitalWrite(LED1,HIGH);

	usleep(2000000);

	
		

		printf("inital LightNumber %d\n",LightNumber);

		while(LightNumber != -1)
		{
			for(; counter != LightNumber ;)
			{
				if ( counter < LightNumber)
				{		
					light(counter);
					usleep(200000);
					counter++;
				}
				else 
				{
					light(counter);
					usleep(200000);
					counter--;
				}
			}

			light(counter);
		}
	

	return 0;


}

void light(int a)
{	
			if ( a/8 ==1 )
			{
				digitalWrite(LED4,HIGH);
			}
			else 
			{
				digitalWrite(LED4,LOW);
			}

			if ( (a%8)/4 ==1 )
			{
				digitalWrite(LED3,HIGH);
			}
			else
			{
				digitalWrite(LED3,LOW);
			}
			
			if ((a%4)/2 == 1)
			{
				digitalWrite(LED2,HIGH);
			} 
			else
			{
				digitalWrite(LED2,LOW);
			}

			if ( a%2 ==1 )
			{
				digitalWrite(LED1,HIGH);
			}
			else
			{
				digitalWrite(LED1,LOW);
			}

}