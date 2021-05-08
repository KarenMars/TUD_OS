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

#define THREAD 4


void * AddThread(void *param);
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

//Adds a new node to the back of the queue
void addToQueue(struct Queue* queue, int brightness, int duration) 
{
	pthread_mutex_lock(&queue->mutex);

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

}


void removeFromQueue(struct Queue* queue, int* pBrightness, int* pDuration) 
{
	pthread_mutex_lock(&queue->mutex);
	/*check whether it is empty*/
	if (queue->front == NULL && queue->back == NULL)
	{	
		/*empty*/
		printf("the queue is empty!!!!\n");
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
			printf("remove last node\n");
		}

		free(DeNode);

	}

 	pthread_mutex_unlock(&queue->mutex);

}


/*---------------NO POINTER TO BRIGHTNESS & DURATION*/
// void removeFromQueue(struct Queue* queue) 
// {
// 	pthread_mutex_lock(&queue->mutex);
// 	/*check whether it is empty*/
// 	if (queue->front == NULL && queue->back == NULL)
// 	{	
// 		/*empty*/
// 		printf("the queue is empty!!!!\n");
		
// 	}
// 	else
// 	{
// 		Node *DeNode = queue->front;
// 		queue->front = DeNode->next;
// 		/*only one element left*/
// 		if (queue->back == DeNode)
// 		{
// 			queue->front = queue->back = NULL;
// 		}
// 		free(DeNode);

// 	}

//  	pthread_mutex_unlock(&queue->mutex);

// }

//Returns the length of the queue
int queueSize(struct Queue* queue) 
{
	int length = 0;
	Node *SearchNode = queue->front;
	while( SearchNode != NULL)
	{
		length++;
		SearchNode = SearchNode->next;
	}

	return length;

}
//Initializes the values of an already allocated queue struct
void initQueue(struct Queue* queue) 
{ 
	queue->front=0;
	queue->back=0;

}


void *AddThread(void *param)
{
	int i = 0;
	printf("add thread \n");
	for (i=0; i<100000; i++)
	{
		addToQueue(param,2,2);
	}
	printf("add thread exit\n");
	return 0;
}


void *SubThread(void *param)
{
	printf("sub thread\n");
	int i = 0;

	int* pBrightness;
	int* pDuration;

	/*allocate memory to two pointers*/
	pBrightness = malloc(sizeof(int));
	pDuration = malloc(sizeof(int));

	for (i=0; i<100000;i++)
	{
		//printf("test remove or not\n");
		removeFromQueue(param,pBrightness,pDuration);
		if (*pBrightness == -1 && *pDuration == -1)
		{
			printf("The queue is empty!!!\n");
		}

	}

	free(pBrightness);
	free(pDuration);

	return 0;
}


int main(void)
{

	Queue* MyQueue;

	int threads = THREAD;
	int length = 0;
	pthread_t * thread = malloc(sizeof(pthread_t)*threads);

	/*allocate memory for a new queue*/
	MyQueue = (Queue*)malloc(sizeof(Queue));

	if ( MyQueue == NULL)
	{	
		fprintf(stderr, "Insufficient memory for creating a new queue!\n");
	}

	/*initialize*/
	initQueue(MyQueue);

	/*initialize Myqueue->mutex*/
	pthread_mutex_init(&MyQueue->mutex,0);

	/*spawn thread*/
	pthread_create(&thread[0],NULL,AddThread,(void *)MyQueue);
	pthread_create(&thread[1],NULL,AddThread,(void *)MyQueue);
	pthread_join(thread[0],NULL);
	pthread_join(thread[1],NULL);

	length = queueSize(MyQueue);

	printf("The size of MyQueue is %d\n", length);

	pthread_create(&thread[2],NULL,SubThread,(void *)MyQueue);
	pthread_create(&thread[3],NULL,SubThread,(void *)MyQueue);
	pthread_join(thread[2],NULL);
	pthread_join(thread[3],NULL);

	length = queueSize(MyQueue);

	printf("The size of MyQueue is %d\n", length);
 	pthread_mutex_destroy(&MyQueue->mutex);

	return 0;
}
