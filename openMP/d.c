#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>

#define N 100000
#define numthreads 10

int cmp_init_asc(const void *p1, const void *p2)
{
	const int *a = (const int *)p1;
	const int *b = (const int *)p2;

	if(*a < *b) return -1;
	if(*a > *b) return 1;
	return 0;
}

int main(){
	static int arre[N];
	srand ((unsigned)time(NULL));
	for(int i = 0; i < N; i++)
		arre[i] = rand() % 100000;

	omp_set_num_threads(numthreads);
	#pragma omp parallel shared(arre)
	{
		#if defined (_OPENMP)
			int iam = omp_get_thread_num();
			int nthreads = omp_get_num_threads();
			int rebanada = N / nthreads;
			int inicio = iam * rebanada;
			int end;
			if(iam == nthreads - 1)
				end = N;
			else
				end = inicio + rebanada;
		#endif
		qsort(&arre[inicio], rebanada, sizeof(int), cmp_init_asc);
		#pragma omp critical
		{
			printf("Soy el Thread: %d | [%d - %d]\n", iam, inicio, end);
			for(int i = inicio; i < end; i++)
				printf("%7d", arre[i]);
			printf("\n\n");
		}
	}


	return 0;
}
