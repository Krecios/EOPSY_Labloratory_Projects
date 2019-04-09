#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <wait.h>

#define SIGNALOPT   //definig the signal mode for the code, without it the program will run without custom signals

#ifdef SIGNALOPT
int InteruptFlag = 0; 
void KeyboardStop()     //custom interupt function that prints the interupt message ands sets up the flag for later termination of processes
{
    printf("KEYBOARD INTERUPT DETECTED\n");
    InteruptFlag = 1;
}
void Termination()  //custom sigterm signal
{
    printf("Parent: [%i] has beent terminated using Termination()\n", getpid());
}
#endif

int main()
{

    int NUM_CHILD = 5;
    int counter = 0;
    struct sigaction sig;   //settin up the sigaction object that will be used in the signal processing

    #ifdef SIGNALOPT
    sig.sa_handler = SIG_IGN;   //ignoring all the signals in a for loop (NSIG is a integer number of created signals)
    for(int i = 0; i<NSIG; i++) sigaction(i, &sig, 0);

    sig.sa_handler = SIG_DFL;   //restoring the default handler for ingnored signals (SIG_DFL is a signal default mode, it's used to bring the ignored signals to the deafult mode)
    sigaction(SIGCHLD, &sig, 0);

    sig.sa_handler = KeyboardStop; //setting up the custom keyboard interupt that will react when Cntrl+C i pressed on the keyboard
    sigaction(SIGINT, &sig, 0);
    #endif

    for(int i=0; i<NUM_CHILD; ++i) //setting up loop for given number of processes
    {
    #ifdef SIGNALOPT
    if(InteruptFlag == 1)   //if the keyboard interupt was detected, the porgram displays an error message and terminates the created processes
    {
        printf("Interuption of parent: [%i] has been detected\n",getpid());
        kill(0, SIGTERM);
        break;
    }
    #endif
	switch(fork())
{
        case 0:  //checking if the signal was properly created
        {
            #ifdef SIGNALOPT
            sig.sa_handler = SIG_DFL;   //restoring the SIGINT signal back to the default one
            sigaction(SIGINT, &sig, 0);
            sig.sa_handler = Termination;   //setting a custom SIGTERM signal
            sigaction(SIGTERM, &sig, 0);
            #endif
            printf("Parent: [%i], Child: [%i] created\n", getppid(), getpid());
            sleep(10);  //waiting before the process termination
            exit(0);
        }
	break;
        case -1: //checking if signal was not properly created
        {
	    kill(0, SIGTERM);   //terminating all the created processes
        exit(1);
        }
	break;
}
sleep(1);
}
    while(1)
        {
            int WaitToDie = wait(0);  //waiting for a child to die and transfering the pid of last dead child to the int WaitToDie
            if(WaitToDie == -1)   //if error occurs, or there is no more children
            {
                break;
            }
            else //when the child is tenminated
            {
                printf("Child: [%i] has died\n",WaitToDie);
                counter++;
            }
            
        }
    printf("Finally, [%i] children has died\n", counter);
    #ifdef SIGNALOPT
    sig.sa_handler = SIG_DFL;   //restoring all the default handlers for the signals
    for(int i = 0; i<_NSIG; i++) sigaction(i, &sig, 0);
    #endif
}
