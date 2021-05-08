#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define LED1 7
#define LED2 0
#define LED3 2
#define LED4 3

#define THREAD 5

void * SubThread(void *param);

/*--------define node--------*/
typedef struct Node Node;

struct Node 
{
int brightness, duration;//Stored values
struct Node* next;//Next node in line
};

/*--------define Queue--------*/
typedef struct Queue Queue;

struct Queue 
{
struct Node* front;
struct Node* back;
pthread_mutex_t mutex; 
};

/*---------define parameter structure for subthread-------------------*/
typedef struct myparam myparam;

struct myparam
{
	Queue* queue; // first param
	int which; //second param
};





//Adds a new node to the back of the queue
void addToQueue(struct Queue* queue, int brightness, int duration) 
{
	pthread_mutex_lock(&queue->mutex);
	printf("lock\n");
	Node *NewNode=(Node*)malloc(sizeof(Node));
	/*assign area of the new node*/

	NewNode->brightness = brightness;
	NewNode->duration = duration;
	NewNode->next = NULL;

	/*check whether it is the first node*/
	if (queue->front == NULL && queue->back == NULL)
	{
		/*add first node*/
		queue->front = NewNode;
		queue->back = NewNode;
		printf("test first newnode\n");
	}
	else
	{	
		queue->back->next = NewNode;
		queue->back = queue->back->next;
		//printf("test\n");
	}

	pthread_mutex_unlock(&queue->mutex);
	printf("unlock\n");

}


void removeFromQueue(struct Queue* queue, int* pBrightness, int* pDuration) 
{
	pthread_mutex_lock(&queue->mutex);
	/*check whether it is empty*/
	if (queue->front == NULL && queue->back == NULL)
	{	
		/*empty*/
		printf("removeFromQueue: the queue is empty\n");
		*pBrightness = -1;
		*pDuration = -1;
	}
	else
	{
		Node *DeNode = queue->front;
		*pBrightness = DeNode->brightness;
		*pDuration = DeNode->duration;
		queue->front = DeNode->next;

		/*only one element left*/
		if (queue->back == DeNode)
		{
			queue->front = queue->back = NULL;
			printf("removeFromQueue: remove last node\n");
		}

		free(DeNode);

	}

 	pthread_mutex_unlock(&queue->mutex);

}


//Initializes the values of an already allocated queue struct
void initQueue(struct Queue* queue) 
{ 
	queue->front=0;
	queue->back=0;

}

void *SubThread(void *param)
{
	// wiringPiSetup();
	// pinMode(LED1,OUTPUT);
	// pinMode(LED2,OUTPUT);
	// pinMode(LED3,OUTPUT);
	// pinMode(LED4,OUTPUT);

	printf("sub thread\n");
	int i = 0;

	int* pBrightness;
	int* pDuration;

	myparam *this_param; 
	this_param = (myparam*)param;

	Queue* queue = this_param->queue;
	int which = this_param->which;

	/*allocate memory to two pointers*/
	pBrightness = malloc(sizeof(int));
	pDuration = malloc(sizeof(int));

	int a = 0;

	for (i=0; ;i++)
	{
		char dest[4];
		char string[1];
		sprintf(string,"%d",which+1);
		strcpy(dest,"LED");
		strcat(dest,string);
		printf("%s\n",dest);
		int num;

		if (strcmp(dest,"LED1")==0)
			num = 7;
		if (strcmp(dest,"LED2")==0)
			num = 0;
		if (strcmp(dest,"LED3")==0)
			num = 2;
		if (strcmp(dest,"LED4")==0)
			num = 3;


		removeFromQueue(queue,pBrightness,pDuration);

		if ( *pBrightness != -1)
		{
			softPwmWrite(num,*pBrightness);
			a = *pDuration;
			a = a*1000;
			usleep(a);
		}
		else
		{
			digitalWrite(num,LOW);
			return 0;
		}

			// if ( which == 0 )
			// {
			// 	if ( *pBrightness != -1)
			// 	{
			// 	softPwmWrite(LED1,*pBrightness);
			// 	a = *pDuration;
			// 	a = a*1000;
			// 	usleep(a);
			// 	}
			// 	else
			// 	{
			// 		return 0;
			// 	}
			
			// }

			// if ( which == 1 )
			// {
			// 	if ( *pBrightness != -1)
			// 	{
			// 	softPwmWrite(LED2,*pBrightness);
			// 	a = *pDuration;
			// 	a = a*1000;
			// 	usleep(a);
			// 	}
			// 	else
			// 	{
			// 		return 0;
			// 	}
			
			// }

			// if ( which == 2 )
			// {
			// 	if ( *pBrightness != -1)
			// 	{
			// 	softPwmWrite(LED3,*pBrightness);
			// 	a = *pDuration;
			// 	a = a*1000;
			// 	usleep(a);
			// 	}
			// 	else
			// 	{
			// 		return 0;
			// 	}
			
			// }

			// if ( which == 3)
			// {
			// 	if ( *pBrightness != -1)
			// 	{
			// 	softPwmWrite(LED4,*pBrightness);
			// 	a = *pDuration;
			// 	a = a*1000;
			// 	usleep(a);
			// 	}
			// 	else
			// 	{
			// 		return 0;
			// 	}
				
			// }

	}

	free(pBrightness);
	free(pDuration);

	return 0;
}



void *ReadThread(void *param)
{

	Queue** Q;

	Q = (Queue**)param; 


	int a,b,c;


	while ( scanf("%d%d%d",&a,&b,&c) != EOF )
	{	
		//printf("%d%d%d\n",a,b,c);
		addToQueue(Q[a],b,c); 
	}


	return 0;

}




/*create corresponding queue*/
void CreateQueue(struct Queue** queue)
{

	*queue = (Queue*)malloc(sizeof(Queue));

	if (*queue == NULL)
	{
		fprintf(stderr, "Insufficient memory for creating a new queue!\n");
	}

	initQueue(*queue);

}




int main(int argc, char **argv)
{

	/*Setup WiringPi*/
	wiringPiSetup();
	/*initialise pin for PWN output*/
	pinMode(LED1,OUTPUT);
	pinMode(LED2,OUTPUT);
	pinMode(LED3,OUTPUT);
	pinMode(LED4,OUTPUT);

	digitalWrite(LED1,HIGH);
	digitalWrite(LED2,HIGH);
	digitalWrite(LED3,HIGH);
	digitalWrite(LED4,HIGH);

	usleep(2000000);

	digitalWrite(LED1,LOW);
	digitalWrite(LED2,LOW);
	digitalWrite(LED3,LOW);
	digitalWrite(LED4,LOW);

	usleep(2000000);

	softPwmCreate(LED1,0,100);
	softPwmCreate(LED2,0,100);
	softPwmCreate(LED3,0,100);
	softPwmCreate(LED4,0,100);

	/*create four queues for LED1 to LED4*/
	int i = 0;
	int leds = 4;
	/*allocate memory for queues*/
	Queue** queue = malloc(sizeof(Queue*)*leds);

	for (i=0; i<leds; i++)
	{
		printf("Create a queue for LED%d\n",i+1);
		CreateQueue(&queue[i]);
	}
	
	pthread_t input_thread;

	for (i=0;i<leds;i++)
	{
		pthread_mutex_init(&queue[i]->mutex,0);
	}


	pthread_create(&input_thread,NULL,ReadThread,(void *)queue);
	pthread_join(input_thread,NULL);


	/*create four threads for four leds to fetch brightness and duration to light the corresponding led*/

	int j = 0;
	int k = 0;
	int threads = 4;
	pthread_t * thread = malloc(sizeof(pthread_t)*threads);

	myparam d_param[4];


	for (j=0; j<threads; j++)
	{
		printf("Create a thread to set the LED%d\n",j+1);
		printf("SubThread%d\n",j+1);
		d_param[j].queue = queue[j];
		d_param[j].which = j;
		pthread_create(&thread[j],NULL,SubThread,&(d_param[j]));
	}

	for (k=0; k<threads; k++)
	{
		//printf("Finish LED%d\n",k);
		pthread_join(thread[k],NULL);
		printf("Finish LED%d\n",k+1);
		pthread_mutex_destroy(&queue[k]->mutex);
	}

	return 0;
}


