#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{	
	pid_t pid;

	pid = fork();
	// child process because the return value zero
	if ( pid == 0)
	{
/*
child process create a new directory and list the current directory 
*/	
	printf("child process: create new directory\n");
 	system("mkdir TestNewDirectory");
    system("ls");
	return 0;
	}

	// parent process 
	else
	{

	printf("parent process: wait for a child process to terminate\n");
	// waitpid(pid_t pid, int * status, int options)
	waitpid(pid,NULL,0);
	return 0;
	}

}
