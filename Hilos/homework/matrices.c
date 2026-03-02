#include<stdio.h>
#include<pthread.h>

int a[4][4];
int a00[2][2];
int a01[2][2];
int a10[2][2];
int a11[2][2];

int b[4][4];
int b00[2][2];
int b01[2][2];
int b10[2][2];
int b11[2][2];

int c[4][4];
int c00[2][2];
int c01[2][2];
int c10[2][2];
int c11[2][2];

typedef struct
{
	int (*m1)[2][2];
	int (*m2)[2][2];
	int (*m3)[2][2];
	int (*m4)[2][2];
	int (*mc)[2][2];
	/*int (*h12)[2][2];
	int (*h21)[2][2];
	int (*h22)[2][2];*/
}matrices;

void multMatricesxZ(int (*arr)[2][2], int (*arr2)[2][2], int (*arr3)[2][2], int (*arr4)[2][2], int (*arrF)[2][2]);
void *matrizR(void *);
	
pthread_t thread1, thread2, thread3, thread4, thmain;
pthread_mutex_t exmut;

void multMatricesxZ(int (*arr)[2][2], int (*arr2)[2][2], int (*arr3)[2][2], int (*arr4)[2][2], int (*arrF)[2][2])
{
	int arreP[2][2];
	int arreP2[2][2];
	for(int i = 0; i < 2; i++)
		for(int k = 0; k < 2; k++)
			for(int j = 0; j < 2; j++)
				arreP[i][j] += (*arr)[i][k] * (*arr2)[k][j];
	
	for(int i = 0; i < 2; i++)
		for(int k = 0; k < 2; k++)
			for(int j = 0; j < 2; j++)
				arreP2[i][j] += (*arr3)[i][k] * (*arr4)[k][j];

	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
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

	printf("Llena la matriz A\n");
	
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			printf("A[%d][%d]= ", i, j);
			scanf("%d", &a[i][j]);
		}
	printf("\n");
	printf("Llena la matriz B\n");
	
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			printf("B[%d][%d]= ", i, j);
			scanf("%d", &b[i][j]);
		}
	printf("\n");

	//Separar matrices
		//a
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
			a00[i][j] = a[i][j];
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
			a01[i][j] = a[i][j + 2];
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
			a10[i][j] = a[i + 2][j];
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
			a11[i][j] = a[i + 2][j + 2];
		//b
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
			b00[i][j] = b[i][j];
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
			b01[i][j] = b[i][j + 2];
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
			b10[i][j] = b[i + 2][j];
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
			b11[i][j] = b[i + 2][j + 2];
	matrices hilo1;
	hilo1.m1 = &a00;
	hilo1.m2 = &b00;
	hilo1.m3 = &a01;
	hilo1.m4 = &b10;
	hilo1.mc = &c00;
	matrices hilo2;
	hilo2.m1 = &a00;
	hilo2.m2 = &b01;
	hilo2.m3 = &a01;
	hilo2.m4 = &b11;
	hilo2.mc = &c01;
	matrices hilo3;
	hilo3.m1 = &a10;
	hilo3.m2 = &b00;
	hilo3.m3 = &a11;
	hilo3.m4 = &b10;
	hilo3.mc = &c10;
	matrices hilo4;
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
	
	//Llenamos c[4][4]
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 2; j++)
		{
			c[i][j] = c00[i][j];
			c[i][j + 2] = c01[i][j];
			c[i + 2][j] = c10[i][j];
			c[i + 2][j + 2] = c11[i][j];
		}
	//Imprimimos la matriz Resultante
	printf("\nAxB =\n");
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
			printf("%4d ", c[i][j]);
		printf("\n");
	}
	pthread_exit(NULL);
}
