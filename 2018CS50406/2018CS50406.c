/*

COL380. assignment 1.

Deadline: 12.03.2021

Problem: 

Compute the sum of the First N natural numbers. 

The sequential version is provided below.

1. Develop two parallel versions of the given program using OpenMP `parallel for' with maximum 8 threads.

Compilation command: gcc -O0 ...

2. The speedup and efficiency of both parallel versions for 2, 4, 8 threads for N=10^3, N=10^5 and N=10^7 numbers.

3. Does the speedup follow Amdahl's law?

Hint: Use a time function in the program (e.g. omp_get_wtime) to measure the time.

*/

#include <omp.h>   
#include <stdio.h> 
#include <stdlib.h> 


unsigned long long parallel_1_approach(unsigned long long val, int p){

	unsigned long long sum1 = 0;

	unsigned long long *thread_sum;

	omp_set_num_threads(p);

	thread_sum = (unsigned long long *)malloc(sizeof(unsigned long long)*p);

	#pragma omp parallel
    {
   		int thread_num = omp_get_thread_num();
   		thread_sum[thread_num] = 0;
   		#pragma omp for
   		for(int j = 0; j < val; j++)
   		{
   			thread_sum[thread_num] += j + 1;
   		}
    }      

    for(int k = 0; k < p; k++)
    	sum1 += thread_sum[k];  

    free(thread_sum);

    return sum1;

}

unsigned long long parallel_2_approach(unsigned long long val, int p){

	unsigned long long *num_list;

	omp_set_num_threads(p);

	num_list = (unsigned long long *)malloc(sizeof(unsigned long long)* val);

	for(int i = 0; i < val; i++){
		num_list[i] = i + 1;
	}
   
    unsigned long long div_factor = 1;

    while(val > div_factor){   	
       	#pragma omp parallel for
       	for(unsigned long long i = 0; i < val; i += 2*div_factor){
        	if(i + div_factor < val) 
       			num_list[i] = num_list[i + div_factor] + num_list[i];
    	}   
      	div_factor = div_factor*2;
   	}

   	unsigned long long ret = num_list[0];

   	free(num_list);

   	return ret;
}

int main(int argc, char* argv[]) 
{ 
	unsigned long long sum = 0;
    unsigned long long  *array;
    unsigned long long N = atoi(argv[2]); 
    int approach = atoi(argv[1]);   // 0,1
    int p = atoi(argv[3]);      	

    // printf("%s %d\n", "p =", p);
   	// printf("%s %d\n", "approach =", approach);
   	// printf("%s %llu\n", "N =", N);
   	sum = parallel_2_approach(N, p);

   	if(approach == 0){
   		sum = parallel_1_approach(N, p);
   		printf("%llu\n",sum);	
   	}
   	else if (approach == 1){
   		sum = parallel_2_approach(N, p);
   		printf("%llu\n",sum);
   	}
   	else{
   		// printf("%s\n", "Enter 1 or 2 approahes only.");
   	}
   	  	

}