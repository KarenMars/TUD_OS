#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

//define led output pins
#define LED1 7
#define LED2 0
#define LED3 2
#define LED4 3

int main(void)
{
	//Set up WiringPi
	wiringPiSetup();
	//Set pin to output
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);

	int LightNumber = -1;
	//give input to the led light
	while ( LightNumber<0 || LightNumber>15)
	{
		printf("give a number between 0 to 15 to set the status of the led light\n");
		scanf("%d",&LightNumber);
		if ( LightNumber<0 || LightNumber>15 )
		printf("Out of Range!!! Please give a right number\n");
	}


	light(LightNumber);
	return 0;

}

int light(int a )
{	
	//create a array to store the Binary Number
	int b[4];	
	int i = 0;
	int c = 0;

	printf("convert the decimal LightNumber to Binary\n");
	
	b[3] = a/8;
	b[2] = (a%8)/4;
	b[1] = (a%4)/2;
	b[0] = a%2;
	
	for (c=3;c>=0;)
	{
		printf("%d",b[c]);
		c--;
		//print the binary number 
	}

	if ( b[3] == 1)
	{
		digitalWrite(LED4,HIGH);
	}
	else 
	{
		digitalWrite(LED4,LOW);
	}


	if ( b[2] == 1)
	{
		digitalWrite(LED3,HIGH);
	}
	else 
	{
		digitalWrite(LED3,LOW);
	}


	if ( b[1] == 1)
	{
		digitalWrite(LED2,HIGH);
	}
	else 
	{
		digitalWrite(LED2,LOW);
	}


	if ( b[0] == 1)
	{
		digitalWrite(LED1,HIGH);
	}
	else 
	{
		digitalWrite(LED1,LOW);
	}

	return 0;
}
