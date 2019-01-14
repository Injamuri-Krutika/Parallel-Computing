# include <stdlib.h>
# include <stdio.h>
# include <omp.h>

void prime_number ( int n ){
	int i,j,prime,total = 0,count=0;
	# pragma omp parallel for\
	shared ( n ) \
	private ( i, j, prime )\
	reduction ( + : total )
	for ( i = 2; i <= n; i++ ){
		prime = 1;
		for (j = 2; j < i; j++ ){
			if ( i % j == 0 ){
				prime = 0;
				break;
			}
		}
		if(prime && count==0){
			printf("%d ",i);
			//count++;		
		}
		
		total = total + prime;
	}
	printf("\n");
}

int main ( int argc, char *argv[] ){
	int primes;
	double wtime;

	for(int k=1;k<=1000;k=k*4)
	{
		printf("\n Number of threads being used = %d\n",k);
		omp_set_num_threads (k);
		for(int i=3;i<=100;i=i*2){
			wtime = omp_get_wtime ( );
			printf("Primes within %d are : ",i);
			prime_number ( i );
			wtime = omp_get_wtime ( ) - wtime;
			printf ( "Max limit : %8d\tTime Take :%14f\n", i, wtime );
		}
		printf("-------------------------------------------");
	}
	return 0;
}

