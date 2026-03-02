#include <stdio.h>
#include <stdlib.h>

#define nThreads 8

int main(){
	static int A[nThreads][nThreads], B[nThreads][nThreads], C[nThreads][nThreads];
	
	for(int i = 0; i < nThreads; i++)
		for(int j = 0; j < nThreads; j++)
		{
			printf("A[%d][%d] = ", i, j);
			scanf("%d", &A[i][j]);
		}
	printf("\n\n");
	for(int i = 0; i < nThreads; i++)
		for(int j = 0; j < nThreads; j++)
		{
			printf("B[%d][%d] = ", i, j);
			scanf("%d", &B[i][j]);
		}
	
	#pragma omp parallel for shared(A, B, C)
	for(int i = 0; i < nThreads; i++)
		for(int j = 0; j < nThreads; j++)
		{
			int suma = 0;
			for(int k = 0; k < nThreads; k++)
				suma += A[i][k] * B[k][j];
			C[i][j] = suma;
		}
	
	printf("Matriz resultant: \n ");
	for(int i = 0; i < nThreads; i++)
	{
		for(int j = 0; j < nThreads; j++)
			printf("%6d " , C[i][j]);
		printf("\n");
	}


	return 0;
}
