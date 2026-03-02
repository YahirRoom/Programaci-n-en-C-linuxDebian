#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define N 10000

int menor = 0, indice = 0; //Variable compartida

/*Prototipos de las funciones que ejecutan los threads*/
void *encontrarMen (void *);
//Estructura que contiene los rangos del arreglo para sacar el menor
typedef struct
{
	int lim_inf, lim_may, *a;
}limites;

/*Declaración de los threads*/
pthread_t thread1, thread2, thread3, thread4, thread5, thmain;
pthread_attr_t attr;//Atributos de los treads
/*Declaración del mutex*/
pthread_mutex_t exmut;


void *encontrarMen (void *arg)
{
	int liminf, limay, localMen;
	pthread_t tid = pthread_self();
	limites *lims = (limites *)(arg);
	liminf = (lims->lim_inf);
	limay = (lims->lim_may);
	localMen = lims->a[liminf];
	for(int i = liminf + 1; i < limay; i++)
		if(lims->a[i] < localMen)
			localMen = lims->a[i];
	
	
	pthread_mutex_lock(&exmut);
	menor = lims->a[liminf];
	if(localMen < menor)
	{
		menor = localMen;
		indice++;
	}
	printf("T%d min[%d-%d] = %d\n", indice,liminf,limay,menor);
	pthread_mutex_unlock(&exmut);
	pthread_exit(NULL);
}

int main()
{	
	static int arre[N];
	
	srand((unsigned)time(NULL));
	for(int i = 0; i < N; i++)
		arre[i] = (rand() % N) + 1;
	
	limites limitess[5];
	for(int i = 0; i < 5; i++)
		limitess[i].a = arre;
	//Ingreso los limites
	limitess[0].lim_inf = 0;
	limitess[0].lim_may = 2000;
	limitess[1].lim_inf = 2000;
	limitess[1].lim_may = 4000;
	limitess[2].lim_inf = 4000;
	limitess[2].lim_may = 6000;
	limitess[3].lim_inf = 6000;
	limitess[3].lim_may = 8000;
	limitess[4].lim_inf = 8000;
	limitess[4].lim_may = 10000;
	

	pthread_mutex_init(&exmut,NULL);//Inicializo los valores del mutex por defecto
	thmain = pthread_self();
	pthread_attr_init(&attr);
	pthread_create(&thread1, &attr, encontrarMen, &limitess[0]);
	pthread_create(&thread2, &attr, encontrarMen, &limitess[1]);
	pthread_create(&thread3, &attr, encontrarMen, &limitess[2]);
	pthread_create(&thread4, &attr, encontrarMen, &limitess[3]);
	pthread_create(&thread5, &attr, encontrarMen, &limitess[4]);
	pthread_exit(NULL);

	//return 0;
}
