#include<stdio.h>
#include<pthread.h>



typedef struct
{
	int n;
	int (*m1)[n/2][n/2];
	int (*m2)[n/2][n/2];
	int (*m3)[n/2][n/2];
	int (*m4)[n/2][n/2];
	int (*mc)[n/2][n/2];
	/*int (*h12)[2][2];
	int (*h21)[2][2];
	int (*h22)[2][2];*/
}matrices;

void multMatricesxZ(int a, int (*arr)[][], int (*arr2)[][], int (*arr3)[][], int (*arr4)[][], int (*arrF)[][]);
void *matrizR(void *);
	
pthread_t thread1, thread2, thread3, thread4, thmain;
pthread_mutex_t exmut;

void multMatricesxZ(int a, int (*arr)[][], int (*arr2)[][], int (*arr3)[][], int (*arr4)[][], int (*arrF)[][])
{
	int arreP[a][a];
	int arreP2[a][a];
	for(int i = 0; i < a; i++)
		for(int k = 0; k < a; k++)
			for(int j = 0; j < a; j++)
				arreP[i][j] += (*arr)[i][k] * (*arr2)[k][j];
	
	for(int i = 0; i < a; i++)
		for(int k = 0; k < a; k++)
			for(int j = 0; j < a; j++)
				arreP2[i][j] += (*arr3)[i][k] * (*arr4)[k][j];

	for(int i = 0; i < a; i++)
		for(int j = 0; j < a; j++)
			(*arrF)[i][j] += arreP[i][j] + arreP2[i][j];
}

void *matrizR(void *arg)
{
	pthread_t tid = pthread_self();
	matrices *mats = (matrices *)(arg);
	pthread_mutex_lock(&exmut);
	multMatricesxZ(mats->m1,mats->m2,mats->m3,mats->m4,mats->mc);
	pthread_mutex_unlock(&exmut);
	pthread_exit(NULL);
}
int main(){
	static int nxm;
	do{
		printf("Escribe el número de filas y columnas de la matriz: ");
		scanf("%d", &nxm);
		if(nxm % 2 != 0)
			printf("Debe de ser múltiplo de 2, pero no puede ser 2\n");
		if(nxm == 2)
			printf("No puede ser 2\n");
	}while(((nxm % 2) != 0) || (nxm == 2));
		
	static int a[nxm][nxm];
	static int a00[nxm/2][nxm/2];
	static int a01[nxm/2][nxm/2];
	static int a10[nxm/2][nxm/2];
	static int a11[nxm/2][nxm/2];

	static int b[nxm][nxm];
	static int b00[nxm/2][nxm/2];
	static int b01[nxm/2][nxm/2];
	static int b10[nxm/2][nxm/2];
	static int b11[nxm/2][nxm/2];

	static int c[nxm][nxm];
	static int c00[nxm/2][nxm/2];
	static int c01[nxm/2][nxm/2];
	static int c10[nxm/2][nxm/2];
	static int c11[nxm/2][nxm/2];

	printf("Llena la matriz A\n");
	
	for(int i = 0; i < nxm; i++)
		for(int j = 0; j < nxm; j++)
		{
			printf("A[%d][%d]= ", i, j);
			scanf("%d", &a[i][j]);
		}
	printf("\n");
	printf("Llena la matriz B\n");
	
	for(int i = 0; i < nxm; i++)
		for(int j = 0; j < nxm; j++)
		{
			printf("B[%d][%d]= ", i, j);
			scanf("%d", &b[i][j]);
		}
	printf("\n");

	//Separar matrices
		//a
	for(int i = 0; i < (nxm/2); i++)
		for(int j = 0; j < (nxm/2); j++)
			a00[i][j] = a[i][j];
	for(int i = 0; i < (nxm/2); i++)
		for(int j = 0; j < (nxm/2); j++)
			a01[i][j] = a[i][j + (nxm/2)];
	for(int i = 0; i < (nxm/2); i++)
		for(int j = 0; j < (nxm/2); j++)
			a10[i][j] = a[i + (nxm/2)][j];
	for(int i = 0; i < (nxm/2); i++)
		for(int j = 0; j < (nxm/2); j++)
			a11[i][j] = a[i + (nxm/2)][j + (nxm/2)];
		//b
	for(int i = 0; i < (nxm/2); i++)
		for(int j = 0; j < (nxm/2); j++)
			b00[i][j] = b[i][j];
	for(int i = 0; i < (nxm/2); i++)
		for(int j = 0; j < (nxm/2); j++)
			b01[i][j] = b[i][j + (nxm/2)];
	for(int i = 0; i < (nxm/2); i++)
		for(int j = 0; j < (nxm/2); j++)
			b10[i][j] = b[i + (nxm/2)][j];
	for(int i = 0; i < (nxm/2); i++)
		for(int j = 0; j < (nxm/2); j++)
			b11[i][j] = b[i + (nxm/2)][j + (nxm/2)];
	matrices hilo1;
	hilo1.n = (nxm / 2);
	hilo1.m1 = &a00;
	hilo1.m2 = &b00;
	hilo1.m3 = &a01;
	hilo1.m4 = &b10;
	hilo1.mc = &c00;
	matrices hilo2;
	hilo2.n = (nxm / 2);
	hilo2.m1 = &a00;
	hilo2.m2 = &b01;
	hilo2.m3 = &a01;
	hilo2.m4 = &b11;
	hilo2.mc = &c01;
	matrices hilo3;
	hilo3.n = (nxm / 2);
	hilo3.m1 = &a10;
	hilo3.m2 = &b00;
	hilo3.m3 = &a11;
	hilo3.m4 = &b10;
	hilo3.mc = &c10;
	matrices hilo4;
	hilo4.n = (nxm / 2);
	hilo4.m1 = &a10;
	hilo4.m2 = &b01;
	hilo4.m3 = &a11;
	hilo4.m4 = &b11;
	hilo4.mc = &c11;
	
	thmain = pthread_self();
	
	pthread_mutex_init(&exmut, NULL);
	pthread_create(&thread1, NULL, matrizR, &hilo1);
	pthread_create(&thread2, NULL, matrizR, &hilo2);
	pthread_create(&thread3, NULL, matrizR, &hilo3);
	pthread_create(&thread4, NULL, matrizR, &hilo4);
	
	//Nos aseguramos de que los hilos hayan terminado
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	
	//Llenamos c[nxm][nxm]
	for(int i = 0; i < (nxm/2); i++)
		for(int j = 0; j < (nxm/2); j++)
		{
			c[i][j] = c00[i][j];
			c[i][j + (nxm/2)] = c01[i][j];
			c[i + (nxm/2)][j] = c10[i][j];
			c[i + (nxm/2)][j + (nxm/2)] = c11[i][j];
		}
	//Imprimimos la matriz Resultante
	printf("\nAxB =\n");
	for(int i = 0; i < nxm; i++)
	{
		for(int j = 0; j < nxm; j++)
			printf("%4d ", c[i][j]);
		printf("\n");
	}
	pthread_exit(NULL);
}
