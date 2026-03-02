#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>

#define N 100000
#define numThreads 8


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
	static int hist_local[numThreads][N];
	static int hist_global[N];
	int mediana = 0;

	srand ((unsigned)time(NULL));
	for(int i = 0; i < N; i++)
		arre[i] = rand() % 100000;
	size_t n = sizeof(arre)/sizeof(arre[0]);
	qsort(arre, n, sizeof(int), cmp_init_asc);
	
	omp_set_num_threads(numThreads);
	#pragma omp parallel shared(arre, mediana, hist_local)
	{
		#if defined (_OPENMP)
			int tid = omp_get_thread_num();
		#endif
		for(int i = 0; i < N; i++)
			hist_local[tid][i] = 0;
		#pragma omp for
		for(int i = 0; i < N; i++)
		{
			int valor = arre[i];
			hist_local[tid][valor]++;
		}
		#pragma omp single
		{
			mediana = (  ( arre[(N/2) - 1] + arre[N/2] )  / 2  );
		}
	}
	#pragma omp parallel for
	for(int i = 0; i < N; i++)
	{
		int suma = 0;
		for(int j = 0; j < numThreads; j++)
			suma += hist_local[j][i];
		
		hist_global[i] = suma;
	}
	
	int moda = 0;
	int frec_moda = hist_global[0];
	for(int i = 1; i < N; i++)
		if(hist_global[i] > frec_moda)
		{
			frec_moda = hist_global[i];
			moda = i;
		}
	printf("Moda = %d (frecuencia = %d)\n", moda, frec_moda);
	printf("La mediana es: %d\n", mediana);


	return 0;
}
