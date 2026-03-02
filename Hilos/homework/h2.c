#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define N 10000

int indexxx = 0;
int menor[5];

void *menorParcialFind(void *);
void *menorMenoresFind(void *);

typedef struct
{
	int lim_inf, lim_mayor, *a;
}datos;
typedef struct
{
	int *arrayM;
}arregloMenores;

pthread_t thread1,thread2,thread3,thread4,thread5,thread6,thmain;
pthread_mutex_t exmut;

void *menorParcialFind(void *arg)
{
	datos *dates = (datos *)(arg);
	int limite_infe = (dates->lim_inf);
	int menorLocal = (dates->a[limite_infe]);
	pthread_t tid = pthread_self();
	for(int i = (dates->lim_inf) + 1; i < (dates->lim_mayor); i++)
		if((dates->a[i]) < menorLocal)
			menorLocal = (dates->a[i]);
	
	pthread_mutex_lock(&exmut);
	printf("Thread(%d) --- minimo parcial(%d-%d) = %d\n", (indexxx + 1), dates->lim_inf, dates->lim_mayor, menorLocal);
	menor[indexxx] = menorLocal;
	indexxx++;
	pthread_mutex_unlock(&exmut);
	pthread_exit(NULL);
}
void *menorTotalFind(void *arg)
{
	pthread_t tid = pthread_self();
	arregloMenores *listaMen = (arregloMenores *)(arg);
	int err = 0;
	int menorTotal;

	if((pthread_join(thread1,NULL) == 0) && (pthread_join(thread2,NULL) == 0) && (pthread_join(thread3,NULL) == 0) && (pthread_join(thread4,NULL) == 0) && (pthread_join(thread5,NULL) == 0) && (pthread_join(thmain,NULL) == 0))
	{
		pthread_mutex_lock(&exmut);
		menorTotal = menor[0];
		pthread_mutex_unlock(&exmut);

		for(int i = 1; i < 5; i++)
			if(listaMen->arrayM[i] < menorTotal)
				menorTotal = listaMen->arrayM[i];
		printf("-El menor de menores es: %d\n", menorTotal);

	}
	pthread_exit(NULL);
}

int main()
{
	static int arreglo[N];

	srand((unsigned)time(NULL));
	for(int i = 0; i < N; i++)
		arreglo[i] = (rand() % N) + 1;
	
	datos limites[5];
	for(int i = 0; i < 5; i++)
		limites[i].a = arreglo;
	
	//Limites de cada thread
	limites[0].lim_inf = 0;
	limites[0].lim_mayor = 2000;
	limites[1].lim_inf = 2000;
	limites[1].lim_mayor = 4000;
	limites[2].lim_inf = 4000;
	limites[2].lim_mayor = 6000;
	limites[3].lim_inf = 6000;
	limites[3].lim_mayor = 8000;
	limites[4].lim_inf = 8000;
	limites[4].lim_mayor = 10000;
	thmain = pthread_self();
	
	pthread_mutex_init(&exmut, NULL);
	pthread_create(&thread1, NULL, menorParcialFind, &limites[0]);
	pthread_create(&thread2, NULL, menorParcialFind, &limites[1]);
	pthread_create(&thread3, NULL, menorParcialFind, &limites[2]);
	pthread_create(&thread4, NULL, menorParcialFind, &limites[3]);
	pthread_create(&thread5, NULL, menorParcialFind, &limites[4]);
	
	arregloMenores arrMen;
	arrMen.arrayM = menor;
	pthread_create(&thread6, NULL, menorTotalFind, &arrMen);
	pthread_exit(NULL);
	


}
