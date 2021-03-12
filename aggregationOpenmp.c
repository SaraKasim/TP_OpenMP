/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

int N = 64; // The size of the array must be a power of 2
int n = 4; // The number of potential threads must divide N

void printTab(int* T){

    for(int i = 0; i < N; i++)
    {
        printf("%d ", T[i]);
    }
    printf("\n");
} // printTab


void initTab(int* T){

    for(int i = 0; i < N; i++){
        T[i] = i;
    }
} // initTab

int sumTabSeq(int* T){
    omp_set_num_threads(4);

    int sum = 0;
    
    #pragma omp parallel
    {
    
        #pragma omp for schedule(dynamic)
            for(int i = 0; i < N; i++){
             sum += T[i];
             
             printf("Thread rank : %d\n" + omp_get_thread_num());
            }
    }

    
    return(sum);
} // data aggregation (sum)


int sumTabPar(int *T)
{
    int distToNext;
    
    
    for(int nbSteps = 0; nbSteps < log2(N); nbSteps++){
        distToNext = pow((float)2, nbSteps);
        
        #pragma omp parallel for
        for(int i = 0; i < N; i += (int)pow(2,nbSteps+1)){
            #pragma omp critical
            {
                T[i] += T[i+distToNext];
            }
        }

        printf("T after step %d: ", nbSteps);
        printTab(T);
    }
    printf("\n");
    return (T[0]);
}



int sumTabPar2(int *T)
{
    
    for(int i = 0; i < n; i++){
        int rootIndex = (i * N/n);
        
        #pragma omp parallel for 
        for (int j = 1; j < N/n; j++){
            #pragma omp critical
            {
                T[rootIndex] += T[rootIndex + j];
            }
        }
    }

    printf("T before final aggregation: ");
    printTab(T);

    int sum = 0;
    
    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < n; i++)
    {
        sum += T[i * (N/n)];
    }

    return(sum);
}



int sumTabParOpenMp(int *T){
    int sum = 0;
	
	#pragma omp parallel for reduction(+:sum)
	for(int i = 0; i < N; i++){
             sum += T[i];
             
            

    
    return(sum);
}


int main(int argc,char *argv[])
{

    int *T = (int *)malloc(N * sizeof(int));
    initTab(T);
    printTab(T);

    printf("\n=== SUM - SEQ VERSION ===\n");
    printf("Sum of elements of T (seq): %d\n", sumTabSeq(T));

    initTab(T);
    printf("\n=== SUM - PTR JUMPING VERSION ===\n");
    printf("Sum of elements of T (par - ptr jumping): %d\n", sumTabPar(T));

    initTab(T);
    printf("\n=== SUM - PARTIAL AGG VERSION ===\n");
    printf("Sum of elements of T (par - partial agg): %d\n", sumTabPar2(T));

    initTab(T);
    printf("\n=== SUM - OPEN MP AGG VERSION ===\n");
    printf("Sum of elements of T (par - partial agg): %d\n", sumTabParOpenMp(T));

}

