#include <stdio.h>
#include <unistd.h>
int main(void)
{
        char Name[] = "Hang Ji";
        int  StudentNumber = 4759745;
        printf("Name = %s\n",Name);
        printf("StudentNumber = %d\n",StudentNumber);
        printf("Process ID = %d\n",getpid());

}


