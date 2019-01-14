# include <stdlib.h>
# include <stdio.h>
# include <omp.h>

int prime_number ( int n ){
	int i,j,prime,total = 0;
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
		total = total + prime;
	}
	return total;
}

int main ( int argc, char *argv[] ){
	int primes;
	double wtime;

	printf ( "\n  Number of processors available = %d\n", omp_get_num_procs ( ) );
	printf ( "  Number of threads =              %d\n", omp_get_max_threads ( ) );
	int maxThreads=omp_get_max_threads( );
	for(int k=1;k<=1000;k=k*4)
	{
		printf("\n Number of threads being used = %d\n",k);
		omp_set_num_threads (k);
		for(int i=1;i<=131072;i=i*5){
			wtime = omp_get_wtime ( );
			primes = prime_number ( i );
			wtime = omp_get_wtime ( ) - wtime;
			printf ( "  %8d\t%8d\t%14f\n", i, primes, wtime );
		}
	}
	return 0;
}

