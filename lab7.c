#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N			5
#define THINKING 	1 
#define HUNGRY 		2
#define EATING		3 
#define FOOD_LIMIT	4
#define DELAY		4

#define LEFT		(id + N - 1) % N
#define RIGHT		(id + 1) % N

pthread_mutex_t m;//internal mutex
pthread_mutex_t s[N];//array of mutexes
pthread_t p[N];
int	state[N];

void grab_forks(int id);
void put_away_forks(int id);
void test(int i);
void *create_philosopher(int id);

void unlock(pthread_mutex_t *m) { pthread_mutex_unlock(m); }
void lock(pthread_mutex_t *m) { pthread_mutex_lock(m); }

int main()
{
	pthread_mutex_init(&m, NULL);	//initialization of mutex m with no parameters
	unlock(&m);	//as mutex is initally locekd, we unlock it
	for (int i=0; i<N; i++) 
	{ 
		p[i] = THINKING;	//all th philosophers in the array are set to THINKING state
		pthread_mutex_init(&s[i], NULL);	//all the mutexes are being locked
		lock(&s[i]);
	}
	for (int i=0; i<N; i++)
		pthread_create(&p[i], NULL, (void *)create_philosopher, (void *)i);	//the thread is created, and the operations on mutexes begin
	
	for (int i = 0 ; i < N ; i++)
		pthread_join(p[i], NULL);	//after the end of operations, the thread is destroyed

	for (int i = 0 ; i < N ; i++)
		pthread_mutex_destroy(&s[i]);	//as well as mutexes
}

void grab_forks(int id)
{
	lock(&m);	//locking the internal mutex, because only one process should be going at once
		state[id] = HUNGRY;	//changing the mutex state
		test(id);	//the check for available eating  option
	unlock(&m);	//unlocking the internal mutex
	lock(&s[id]);	//locking the philosopher
}

void put_away_forks(int id)
{
	lock(&m);	//same as grab_forks
		state[id] = THINKING;	//chnging the mutex state
		test(LEFT);	//testing both left and right philosopher
		test(RIGHT);
	unlock(&m);	//unlocking the internal mutex
}

void test(int id)
{
	if(	state[id] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) //checking the state of current philosopher, as well as its neighbours
	{
		state[id] = EATING;	//if the check is true, then the state is changed, and the philosopher is unlocked and "starts to eat"
		unlock(&s[id]);
	}
}

void *create_philosopher(int id)
{
	printf("Philosopher [%d] arrives to the table\n", id);
	for(int i=0; i<FOOD_LIMIT; i++)	//amount of meals 
	{
		printf("Philosopher [%d] is thinking\n",id);
		sleep(DELAY);	//thihnkning time
		grab_forks(id);
		printf("Philosopher [%d] is eating his meal\n",id);
		sleep(DELAY);	// eating time
		put_away_forks(id);
		printf("Philosopher [%d] finishes his meal\n",id);
	}
	printf("Philosopher [%d] leaves the table\n", id);
}
