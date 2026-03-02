#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>

#define N 100000

int main(){
	static int arre[N];
	int minGlobal;
	int maxGlobal;
	srand ((unsigned)time(NULL));
	for(int i = 0; i < N; i++)
		arre[i] = rand() % 100000;

	minGlobal = arre[0];
	maxGlobal = arre[0];

	omp_set_num_threads(100);
	#pragma omp parallel reduction(min:minGlobal) shared(arre)
	{
		#pragma omp for
		for(int i = 0; i < N; i++)
			if(arre[i] < minGlobal)
				minGlobal = arre[i];
		#pragma omp for
		for(int i = 0; i < N; i++)
			if(arre[i] > maxGlobal)
				maxGlobal = arre[i];
	}
	printf("El mínimo global es: %d\n", minGlobal);
	printf("El máximo global es: %d\n", maxGlobal);

	/*
	int minimo = arre[0];
	for(int i = 0; i < N; i++)
		if(arre[i] < minimo)
			minimo = arre[i];
	printf("%d\n", minimo);
	int maximo = arre[0];
	for(int i = 0; i < N; i++)
		if(arre[i] > maximo)
			maximo = arre[i];
	printf("%d\n", maximo);
	*/


	return 0;
}
