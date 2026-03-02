#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void *reloj(void *);
void *llenado(void *);
void *controldep(void *);
void *alarma(void *);
void *lanzar(void *);

//Declaración de threads
pthread_t tempo, llenar, thmain, alar, control, start;
pthread_cond_t aviso;
pthread_mutex_t exmut;
//Declaración de las variables globales
int tiempoej = 0, caudal = 0, tiempo = 0, volumen = 0, caudalvaci = 0, umbral = 0, peligro = 0;

static void esperar_aviso()
{
	pthread_mutex_lock(&exmut);
	int temp = tiempo;
	while(tiempo == temp && tiempo < tiempoej)
		pthread_cond_wait(&aviso, &exmut);
	pthread_mutex_unlock(&exmut);
}

void *reloj(void *arg)
{
	for(;;)
	{
		sleep(1);
		pthread_mutex_lock(&exmut);
		if(tiempo >= tiempoej)
		{
			pthread_cond_broadcast(&aviso);
			pthread_mutex_unlock(&exmut);
			pthread_exit(NULL);
		}
		tiempo++;
		pthread_cond_broadcast(&aviso);
		pthread_mutex_unlock(&exmut);
	}
	pthread_exit(NULL);
}
void *llenado(void *arg)
{
	for(;;)
	{
		esperar_aviso();
		pthread_mutex_lock(&exmut);
		if(tiempo >= tiempoej)
		{
			pthread_mutex_unlock(&exmut);
			pthread_exit(NULL);
		}
		if (volumen < peligro)
		{
			volumen += caudal;
			printf("Llenado: Volumen = %d\n", volumen);
		}
		pthread_mutex_unlock(&exmut);
	}
	pthread_exit(NULL);
}
void *controldep(void *arg)
{
	for(;;)
	{
		esperar_aviso();
		pthread_mutex_lock(&exmut);
		if(tiempo >= tiempoej)
		{
			pthread_mutex_unlock(&exmut);
			pthread_exit(NULL);
		}
		if(((tiempo % 2) == 0) && (volumen > umbral) && (volumen > 0))
		{
			volumen -= caudalvaci;
			printf("Control: he reducido volumen a %d\n", volumen);
		}
		pthread_mutex_unlock(&exmut);
	}
	pthread_exit(NULL);
}
void *alarma(void *arg)
{
	for(;;)
	{
		esperar_aviso();
		pthread_mutex_lock(&exmut);
		if(tiempo >= tiempoej)
		{
			pthread_mutex_unlock(&exmut);
			pthread_exit(NULL);
		}
		if(volumen > peligro)
			printf("¡¡URGENTE!!, volumen es mayor que el límite: %d\n", volumen);
		pthread_mutex_unlock(&exmut);
	}
	pthread_exit(NULL);
}
void *lanzar(void *arg)
{
	pthread_t tid = pthread_self();
	pthread_create(&tempo, NULL, reloj, NULL);
	pthread_create(&llenar, NULL, llenado, NULL);
	pthread_create(&control, NULL, controldep, NULL);
	pthread_create(&alar, NULL, alarma, NULL);
	pthread_exit(NULL);
}
int main(){
	
	thmain = pthread_self();
	printf("Tiempo total de simulación: ");
	scanf("%d", &tiempoej);
	printf("Cantidad que aventará la bomba(ltrs/seg): ");
	scanf("%d", &caudal);
	printf("Cantidad para activar el sistema de control(ltrs): ");
	scanf("%d", &umbral);
	printf("Cantidad ltrs a tirar cuando se active control: ");
	scanf("%d", &caudalvaci);
	printf("Cant de ltrs para activar alerta: ");
	scanf("%d", &peligro);
	
	pthread_mutex_init(&exmut, NULL);
	pthread_cond_init(&aviso, NULL);
	
	pthread_create(&start, NULL, lanzar, NULL);
	pthread_exit(NULL);
}

