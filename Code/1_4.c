#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

void *fn(void *ptr)
{
	Printf("create directory\n");
	system("mkdir TestNewNewDirectory");
    system("ls");

}

int main(void)
{

	/*pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine)(void*), void *arg)*/
    pthread_t thread;
    pthread_create(&thread,NULL,fn,NULL);
    /*pthread_join(pthread_t thread, void **status)*/
    pthread_join(thread, NULL);
    return 0;

/*
A child process is created by a parent process. A child process can have multiple 
threads.
A thread is an entiry within a process thaat consists of the schedulable part of 
process
*/
}

