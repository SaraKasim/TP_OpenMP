#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

//So first we have N philosophers
#define NUMBER_PHILO 10

int main(){
	int i;//the position of the philosophers
	
	//as mentionned the forks are represented by OpenMP lock : 
	omp_lock_t forks[NUMBER_PHILO];
	
	//at this state we have to initialized the forks (in parallel)
	#pragma omp parallel for schedule(static) private(i)
	for(i=0; i < NUMBER_PHILO ;i++){
		omp_init_lock(&forks[i]);
	}
	
	#pragma omp parallel for schedule(static) private(i)
	for(i = 0; i<NUMBER_PHILO ; i++){
		//at this state the philosophers are sitting at the round table with a delicious dish

		//now we have to know whitch philosopher is at whitch position 
		//so he can take the left and right forks and then release them once he is done eating
		if(i%2 != 0){
			omp_set_lock(&forks[i]);
			printf("the philosopher %d took the fork on his left '\n", i);
			omp_set_lock(&forks[i+1]);
			printf("the philosopher %d took the fork on his right '\n", i);
			
			//at this state the philosopher is eating;
			
			omp_unset_lock(&forks[i]);
			printf("the philosopher %d released the fork on his left '\n", i);
			omp_unset_lock(&forks[i+1]);
			printf("the philosopher %d released the fork on his right '\n", i);
			printf("The philosopher %d has done eating '\n", i);
		}
		
		else{
			omp_set_lock(&forks[i+1]);
			printf("the philosopher %d took the fork on his right'\n", i);
			omp_set_lock(&forks[i]);
			printf("the philosopher %d took the fork his left '\n", i);
			
			//at this state the philosopher is eating;
			
			omp_unset_lock(&forks[i+1]);
			printf("the philosopher %d released the fork his right '\n", i);
			omp_unset_lock(&forks[i]);
			printf("the philosopher %d released the fork his left '\n", i);
			printf("The philosopher %d has done eating '\n", i);
		}
	}
		//at this state all philosophers has done eating
	printf("All philosophers has done eating");
}

