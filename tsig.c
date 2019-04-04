#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <wait.h>

int main()
{
    int NUM_CHILD = 5;
    int parentPid;
    int counter = 0;
    for(int i=0; i<NUM_CHILD; ++i)
    {
	switch(fork())
{
        case 0:  //checking if the signal was properly created
        {
            printf("Parent: [%i], Child: [%i] created\n", getppid(), getpid());
            sleep(10);
            exit(0);
        }
	break;
        case -1: //checking if signal was not properly created
        {
            printf("ERROR\n");
	    kill(0, SIGTERM);
            exit(1);
        }
	break;
}
        sleep(1);
    }
    while(1)
        {
            int WaitToDie = wait(0);  //waiting for a child to die and transfering the pid of last dead child to the int WaitToDie
            if(WaitToDie == -1)          //if error occurs, or there is no more children
            {
                break;
            }
            else
            {
                printf("Child: [%i] has died\n",WaitToDie);
                counter++;
            }
            
        }
    printf("Finally, [%i] children has died\n", counter);
}
