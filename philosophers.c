#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h> 

#define SemaphoreKey 123456789
#define AmountOfFood 3
#define TimeToEat 1
#define TimeToThink 2

void GrabForks(int LeftForkID);
void PutForksAway(int LeftForkID);
void LifeOfPhilosopher();
void Eat(int MealsLeft);
void PhilosopherIsThinking();

char* ListOfPhilosophers[5] = {"Philosopher1","Philosopher2","Philosopher3","Philosopher4","Philosopher5"};
int PhilosopherID, SemaphorID, pid, status;

int main()
{
    int i = 0;  //loop counter
    SemaphorID = semget(SemaphoreKey, 5, 1234 | IPC_CREAT);  //semget = returns the semaphore identifier associated with key; semflg = combination of <sys/ipc.h> and <sys/modes.h>

    if(SemaphorID == -1)
    {
        exit(0);
    }

    while(i <= 4) semctl(SemaphorID, i++, SETVAL, 1); //semctl - preforms the controll operation on specified semaphor; SEMVAL - sets the avlue of the parameters
    i=0;

    while(i <= 4)
    {
        pid = fork();
        if (pid == 0)
        {
            PhilosopherID = i;
            LifeOfPhilosopher();
            printf("%s: Left the table\n", ListOfPhilosophers[PhilosopherID]);
            return 0;
        }
        else if(pid == -1)
        {
            kill(0,SIGTERM);
            perror("PID ERROR, ABORTING\n");
            exit(1);
        }
        i++;
    }
    while(1)
    {
        pid = wait(&status);
        if(pid == -1) break;
    }
    i = 0;
    if(semctl(SemaphorID, 0, IPC_RMID, 1) < 0) printf("ERROR DELOCATING SEMAPHORS\n");      //remove the semaphor set
    return 0;
}

void GrabForks(int LeftForkID)
{
    int RightForkID = LeftForkID - 1;    //grabbed the fork to the right
    if(RightForkID == -1) {RightForkID = 4;}    //the circular proportion of the table has to be conserved
    printf("%s: Using Left ID: %d, Right ID: %d\n", ListOfPhilosophers[PhilosopherID], LeftForkID, RightForkID);
    struct sembuf SemaphorAsFork[2] = {
        {RightForkID, -1, 0},               //decrease the value of semaphor, because sem_op < 0, sem_flage set to 0 - blocking the operation
        {LeftForkID, -1, 0}};
    semop(SemaphorID, SemaphorAsFork, 2);   //performs operations on the semaphors {semaphorID, semaphorBuffer, NumberOfSemaphors}
}

void PutForksAway(int LeftForkID)
{
    int RightForkID = LeftForkID - 1;    //grabbed the fork to the right
    if(RightForkID == -1) {RightForkID = 4;}    //the circular proportion of the table has to be conserved
    printf("%s: Puts Away Left ID: %d, Right ID: %d\n", ListOfPhilosophers[PhilosopherID], LeftForkID, RightForkID);
    struct sembuf SemaphorAsFork[2] = {
        {RightForkID, 1, 0},               //increase the value of semaphor, because sem_op < 0, sem_flage set to 0 - blocking the operation
        {LeftForkID, 1, 0}};
    semop(SemaphorID, SemaphorAsFork, 2);   //performs operations on the semaphors {semaphorID, semaphorBuffer, NumberOfSemaphors}
}

void LifeOfPhilosopher()
{
    printf("%s: Comes to eat\n", ListOfPhilosophers[PhilosopherID]);
    int NumberOfMeals = AmountOfFood;
    int hungry = 0;
    while(NumberOfMeals != 0)
    {
        if(hungry)
        {
            Eat(--NumberOfMeals);       //postincrementation because it should be substracted aftr the act of eating
            hungry = 0;         //after eating no longer hungry
        }
        else
        {
            PhilosopherIsThinking();
            hungry = 1;         //after thinking hungry again
        }
        
    }
}

void PhilosopherIsThinking()
{
    printf("%s: Is Thinking\n", ListOfPhilosophers[PhilosopherID]);
    sleep(TimeToThink);
}

void Eat(int MealsLeft)
{
    GrabForks(PhilosopherID);
    printf("%s: Is Eating\n", ListOfPhilosophers[PhilosopherID]);
    sleep(TimeToEat);
    printf("%s: Ate meal no. %d\n", ListOfPhilosophers[PhilosopherID], AmountOfFood - MealsLeft);
}
