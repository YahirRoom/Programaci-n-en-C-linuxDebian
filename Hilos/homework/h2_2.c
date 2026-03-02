#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define N 10000

int indexxx = 0;
int sumPar[5];

void *sumaParcial(void *);
void *sumaTotal(void *);

typedef struct
{
	int lim_inf, lim_mayor, *a;
}datos;
typedef struct
{
	int *arrayM;
}sumasParciales;

pthread_t thread1,thread2,thread3,thread4,thread5,thread6,thmain;
pthread_mutex_t exmut;

void *sumaParcial(void *arg)
{
	datos *dates = (datos *)(arg);
	int limite_infe = (dates->lim_inf);
	int sumaParcial;
	pthread_t tid = pthread_self();
	for(int i = dates->lim_inf; i < (dates->lim_mayor); i++)
		sumaParcial += dates->a[i];
	
	pthread_mutex_lock(&exmut);
	printf("Thread(%d) --- suma parcial(%d-%d) = %d\n", (indexxx + 1), dates->lim_inf, dates->lim_mayor, sumaParcial);
	sumPar[indexxx] = sumaParcial;
	indexxx++;
	pthread_mutex_unlock(&exmut);
	pthread_exit(NULL);
}
void *sumaTotal(void *arg)
{
	pthread_t tid = pthread_self();
	sumasParciales *sumsPar = (sumasParciales *)(arg);
	int err = 0;
	if((pthread_join(thread1,NULL) == 0) && (pthread_join(thread2,NULL) == 0) && (pthread_join(thread3,NULL) == 0) && (pthread_join(thread4,NULL) == 0) && (pthread_join(thread5,NULL) == 0) && (pthread_join(thmain,NULL) == 0))
	{

		pthread_mutex_lock(&exmut);
		int sumaTotal = 0;
		pthread_mutex_unlock(&exmut);

		for(int i = 0; i < 5; i++)
			sumaTotal += sumsPar->arrayM[i];
		printf("-La suma total es: %d\n", sumaTotal);

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
	pthread_create(&thread1, NULL, sumaParcial, &limites[0]);
	pthread_create(&thread2, NULL, sumaParcial, &limites[1]);
	pthread_create(&thread3, NULL, sumaParcial, &limites[2]);
	pthread_create(&thread4, NULL, sumaParcial, &limites[3]);
	pthread_create(&thread5, NULL, sumaParcial, &limites[4]);
	
	sumasParciales sumasP;
	sumasP.arrayM = sumPar;
	pthread_create(&thread6, NULL, sumaTotal, &sumasP);
	pthread_exit(NULL);
	


}
