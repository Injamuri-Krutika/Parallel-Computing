#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <omp.h>

void display(int * t, int n){
	for (int i = 0; i < n; ++i){
		printf("%d ", t[i]);
	}
	printf("\n");
}

void prefix_sum(int *tab, int *pprefixsum, unsigned long size){
	int nthr, *z, *x = pprefixsum;
	  #pragma omp parallel
	  {
	    int i;
	    #pragma omp single
	    {
	      nthr = omp_get_num_threads();
	      z = malloc(sizeof(int)*nthr+1);
	      z[0] = 0;
	    }
	    int tid = omp_get_thread_num();
	    int sum = 0;
	    #pragma omp for schedule(static)
	    for(i=0; i<size; i++) {
	      sum += tab[i];
	      x[i] = sum;
	    }
	    z[tid+1] = sum;
	    #pragma omp barrier

	    int offset = 0;
	    for(i=0; i<(tid+1); i++) {
		offset += z[i];
	    }
	    #pragma omp for schedule(static)
	    for(i=0; i<size; i++) {
	      x[i] += offset;
	    }
	  }
	  free(z);
}


int main(int argc, char const *argv[])
{
	
	int maxThreads=omp_get_max_threads( );
	for(int k=1;k<=128;k=k*4)
	{	int count =0;
		printf("\nNumber of threads being used = %d\n",k);
		omp_set_num_threads (k);
		int i=10;
		for(int i=10;i<=1000000;i*=10){
			int * tab = (int *) malloc ((i+1)*sizeof(int));
			int * prefixsum = (int *) malloc ((i+1)*sizeof(int));
			double start,end;
			srand (time(NULL));
			#pragma omp parallel for shared(tab)
			for (int k = 0; k < i; ++k)
			{
				tab[k] = rand()%10;
			}
			tab[i+1] = 0;
			start = omp_get_wtime();
			prefix_sum(tab, prefixsum, i);
			end = omp_get_wtime();
			if(count==0){
				display(tab, i);
				display(prefixsum, i);
				count++;
			}
			printf("%10d : %f\n",i, end - start);
			
			free(tab);
		}
		printf("-----------------------------------------------------------------\n");
	}
		

	return 0;
}
