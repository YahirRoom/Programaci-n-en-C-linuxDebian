#include <stdio.h>
#include <pthread.h>

typedef struct
{
	int n;
	int *m1;
	int *m2;
	int *m3;
	int *m4;
	int *mc;
}matrices;

void multMatrices(int n, int arr[n][n], int arr2[n][n], int arr3[n][n], int arr4[n][n], int *arrc);
void *matrizR(void *);

pthread_t thread1, thread2, thread3, thread4, thmain;


void multMatrices(int n, int arr[n][n], int arr2[n][n], int arr3[n][n], int arr4[n][n], int *arrc)
{
	int arrt[n][n], arrt2[n][n], arrT[n][n];
	int indice = 0;
	
	for(int i = 0; i < n; i++)
		for(int k = 0; k < n; k++)
			for(int j = 0; j < n; j++)
				arrt[i][j] += arr[i][k] * arr2[k][j];

	for(int i = 0; i < n; i++)
		for(int k = 0; k < n; k++)
			for(int j = 0; j < n; j++)
				arrt2[i][j] += arr3[i][k] * arr4[k][j];

	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			arrT[i][j] = arrt[i][j] + arrt2[i][j];
	
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			arrc[indice++] = arrT[i][j];

}
void *matrizR(void *arg)
{
	pthread_t tid = pthread_self();
	matrices *mats = (matrices*)(arg);
	int tam = mats->n;
	int arr[tam][tam], arr2[tam][tam], arr3[tam][tam], arr4[tam][tam];
	int indice = 0;
	for(int i = 0; i < tam; i++)
		for(int j = 0; j < tam; j++)
			arr[i][j] = mats->m1[indice++];
	indice = 0;
	
	for(int i = 0; i < tam; i++)
		for(int j = 0; j < tam; j++)
			arr2[i][j] = mats->m2[indice++];
	indice = 0;
	
	for(int i = 0; i < tam; i++)
		for(int j = 0; j < tam; j++)
			arr3[i][j] = mats->m3[indice++];
	indice = 0;
	
	for(int i = 0; i < tam; i++)
		for(int j = 0; j < tam; j++)
			arr4[i][j] = mats->m4[indice++];



	multMatrices(mats->n, arr, arr2, arr3, arr4, mats->mc);
	pthread_exit(NULL);
}


int main(){
	int n;
	//Preguntamos el tamaño de las matrices cuadradas y verificamos que sean multiplos de 2
	do
	{
		printf("Tamaño de las matrices cuadradas nxn: ");
		scanf("%d", &n);
		if(n == 2 || (n % 2) != 0)
			printf("El tamaño de las matrices debe ser n<2 y multiplo de 2\n");
	}while ((n % 2) != 0 || n == 2);
	//Declaramos las matrices principales y secundiarias
	int n2 = n / 2;
	int a[n][n], b[n][n], c[n][n];
	int mat00[n2][n2], mat01[n2][n2], mat10[n2][n2], mat11[n2][n2],matt00[n2][n2], matt01[n2][n2], matt10[n2][n2], matt11[n2][n2];
	int matc00[n2][n2], matc01[n2][n2], matc10[n2][n2], matc11[n2][n2];
	printf("-----Llenado de matrices A y B-----\n");
	printf("Matriz A\n");
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			printf("A[%d][%d]: ", i, j);
			scanf("%d", &a[i][j]);
		}
	}
	printf("Matriz B\n");
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			printf("B[%d][%d]: ", i, j);
			scanf("%d", &b[i][j]);
		}
	}
	//Dividir y llenar matrices secundarias
		//a -> mat
	for(int i = 0; i < n2; i++)
		for(int j = 0; j < n2; j++)
			mat00[i][j] = a[i][j];

	for(int i = 0; i < n2; i++)
		for(int j = 0; j < n2; j++)
			mat01[i][j] = a[i][j + n2];

	for(int i = 0; i < n2; i++)
		for(int j = 0; j < n2; j++)
			mat10[i][j] = a[i + n2][j];

	for(int i = 0; i < n2; i++)
		for(int j = 0; j < n2; j++)
			mat11[i][j] = a[i + n2][j + n2];

		//b -> matt
	for(int i = 0; i < n2; i++)
		for(int j = 0; j < n2; j++)
			matt00[i][j] = b[i][j];

	for(int i = 0; i < n2; i++)
		for(int j = 0; j < n2; j++)
			matt01[i][j] = b[i][j + n2];

	for(int i = 0; i < n2; i++)
		for(int j = 0; j < n2; j++)
			matt10[i][j] = b[i + n2][j];

	for(int i = 0; i < n2; i++)
		for(int j = 0; j < n2; j++)
			matt11[i][j] = b[i + n2][j + n2];
	
	int *c00 = &matc00[0][0];
	int *c01 = &matc01[0][0];
	int *c10 = &matc10[0][0];
	int *c11 = &matc11[0][0];

	matrices hilo1;
	hilo1.n = n2;
	hilo1.m1 = &mat00[0][0];
	hilo1.m2 = &matt00[0][0];
	hilo1.m3 = &mat01[0][0];
	hilo1.m4 = &matt10[0][0];
	hilo1.mc = c00;
	matrices hilo2;
	hilo2.n = n2;
	hilo2.m1 = &mat00[0][0];
	hilo2.m2 = &matt01[0][0];
	hilo2.m3 = &mat01[0][0];
	hilo2.m4 = &matt11[0][0];
	hilo2.mc = c01;
	matrices hilo3;
	hilo3.n = n2;
	hilo3.m1 = &mat10[0][0];
	hilo3.m2 = &matt00[0][0];
	hilo3.m3 = &mat11[0][0];
	hilo3.m4 = &matt10[0][0];
	hilo3.mc = c10;
	matrices hilo4;
	hilo4.n = n2;
	hilo4.m1 = &mat10[0][0];
	hilo4.m2 = &matt01[0][0];
	hilo4.m3 = &mat11[0][0];
	hilo4.m4 = &matt11[0][0];
	hilo4.mc = c11;
	
	thmain = pthread_self();
	pthread_create(&thread1, NULL, matrizR, &hilo1);
	pthread_create(&thread2, NULL, matrizR, &hilo2);
	pthread_create(&thread3, NULL, matrizR, &hilo3);
	pthread_create(&thread4, NULL, matrizR, &hilo4);
	
	//Nos aseguramos de que todos hayan terminado
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	//Llenamos las 4 partes de C
	int indice = 0;
	for(int i = 0; i < n2; i++)
		for(int j = 0; j < n2; j++)
		{
			matc00[i][j] = c00[indice++];
		}
	indice = 0;
	for(int i = 0; i < n2; i++)
		for(int j = 0; j < n2; j++)
		{
			matc01[i][j] = c01[indice++];
		}
	indice = 0;
	for(int i = 0; i < n2; i++)
		for(int j = 0; j < n2; j++)
		{
			matc10[i][j] = c10[indice++];
		}
	indice = 0;
	for(int i = 0; i < n2; i++)
		for(int j = 0; j < n2; j++)
		{
			matc11[i][j] = c11[indice++];
		}
	indice = 0;
	//Llenamos matc[n][n]
	for(int i = 0; i < n2; i++)
		for(int j = 0; j < n2; j++)
		{
			c[i][j] = matc00[i][j];
			c[i][j + n2] = matc01[i][j];
			c[i + n2][j] = matc10[i][j];
			c[i + n2][j + n2] = matc11[i][j];
		}
	//Imprimimos matriz resultante
	printf("Matriz Resultante C:\n");
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
			printf("%4d", c[i][j]);
		printf("\n");
	}
	return 0;
}
