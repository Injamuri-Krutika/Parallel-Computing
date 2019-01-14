#include<stdio.h>
#include<omp.h>
#include<math.h>

#define PI 3.1415926538837211

int main()
{
	float sum, x, totalsum, h, partialsum, sumthread;
	int maxThreads=omp_get_max_threads( );
	printf("Threads\tIntervals\tPi Value\tError\t\tTime Taken\n");
	for(int k=1;k<=128;k=k*4)
	{
		for(int p=1;p<=1000000;p=p*10){
			double start=omp_get_wtime ( );
			sum=0.0;partialsum=0.0,sumthread=0.0;
			omp_set_num_threads (k);	
			h = 1.0 / p;
			#pragma omp parallel for private(x) reduction(+ : sumthread)
			for (int i = 1; i <=p; i++) {
				x = h * (i - 0.5);
				#pragma omp atomic
				sumthread += 4.0 / (1 + x * x);
			}

			sum = sumthread * h;
			printf("%d\t%d\t\t%f\t%1.10f\t%f\n",k,p,sum, fabs(sum - PI),omp_get_wtime ( )-start);
		}
	printf("----------------------------------------------------------------\n");
	}
	return 0;	
}
