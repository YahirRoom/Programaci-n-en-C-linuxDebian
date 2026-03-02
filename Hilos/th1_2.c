#include <stdio.h>//Entrada/Salida scanf(), printf();
#include <stdlib.h>//Para rand();
#include <pthread.h>
#include <unistd.h>

/*Prototipos de las funciones que ejecutan los threads/ buena práctica de programación*/
void *func1 (void *);
void *func2 (void *);

/*Declaración del nombre de los threads*/
pthread_t thread1, thread2, thmain;
pthread_attr_t attr; //Atributo de los threads

/*Definición función 1 y 2*/
void *func1 (void *arg)
{
	/*Se asigna un identificador a un thread*/
	pthread_t tid = pthread_self();
	printf("Soy el thread 1 y estoy ejecutando Func1 \n");
	sleep(5);
	printf("Soy el thread 1 y he terminado de ejecutar Func1\n");
	/*Al no devolver algún valor se pasa como argumento NULL y provoca la terminación del thread*/
	pthread_exit(NULL);
}
void *func2 (void *arg)
{
	printf("Soy el thread 2 voy a esperar a que thread 1 termine \n");
	int err = pthread_join(thread1, NULL);
	if(err != 0)
		printf("Error al esperar al thread 1\n");
	else
	{
		printf("Soy el thread 2 y estoy ejecutando Func2\n");
		sleep(2);
		printf("Ya terminé jiji\n");
	}
	
	sleep(2);
	
	/*Al no devolver algún valor se pasa como argumento NULL y provoca la terminación del thread*/
	pthread_exit(NULL);
}

//Función main
int main (void)
{
	/*La propia función main es un thread*/
	thmain = pthread_self();
	/*Hay que inicializar SIEMPRE los parametors de los threads por defecto*/
	pthread_attr_init(&attr);
	printf("Soy la función MAIN y voy a lanzar 2 threads\n");
	pthread_create(&thread1, &attr, func1, NULL);
	pthread_create(&thread2, &attr, func2, NULL);
	printf("Soy MAIN: he lanzado los 2 threads y ya terminé\n");
	pthread_exit(NULL);
}


