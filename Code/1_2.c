
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
int main(void)
{

/*
invole shell function to create new directory and list all files and directoriess
 in the current directories
*/

/*      system("mkdir TestDirectory");
        system("ls");
*/

/*
The functionality for interaction with files and folders from the C standard libb
raries
*/

        mkdir("TestDirectory",0777);
/*
 list all files and directories in the current directory
*/

        DIR *direction = opendir(".");
        struct dirent *dp;
        if (direction)
        {
                while((dp=readdir(direction))!=NULL)
                {
                        printf("%s\n",dp->d_name);
                }

        }
        closedir(direction);
        return 0;

}
