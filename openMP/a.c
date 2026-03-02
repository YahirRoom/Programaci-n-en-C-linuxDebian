#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>

#define N 100000

int main(){
	static int arre[N];
	int sumTotal = 0;
	srand ((unsigned)time(NULL));
	for(int i = 0; i < N; i++)
		arre[i] = rand() % 100000;

	omp_set_num_threads(10);
	#pragma omp parallel reduction(+:sumTotal) shared(arre)
	{
		#pragma omp for
		for(int i = 0; i < N; i++)
			sumTotal += arre[i];
	}
	printf("La suma total es: %d\n", sumTotal);



	return 0;
}
