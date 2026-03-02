/*Programa que muestra el paso de parámetros en treads: 
					compilar con: gcc th3.c -o th3 -lpthread*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*Prototipos de las funciones a ejecutar con treads*/
void *func1 (void*);

/*Estructura que contiene los datos a pasar como parámetros, sólo se puede pasar un único parámetro con el 
operador "&"*/
typedef struct
{
	int dato1, dato2;
}datos;

/*Declaración de los threads*/
pthread_t thread1, thmain;
//pthread_attr_t attr; //Podría poner esta línea pero lo haré de forma sencilla, con el argumento: NULL


/*Definición de la función Func1*/

void *func1 (void *arg)
{
	int a, b;
	datos *p = (datos *) (arg);
	/*Se asigna un identificador de thread*/
	pthread_t tid = pthread_self();
	a = (p->dato1);
	b = (p->dato2);
	printf("Soy el thread 1 y voy a ejecutar func 1\n");
	printf("La multiplicación con los parámetros es: %d\n", a*b);
	printf("Soy el thread 1 y he terminado de ejecutar la función 1\n");
	pthread_exit(NULL);
}
/*Función main*/
int main(void)
{
	datos param;
	param.dato1 = 5;
	param.dato2 = 4;
	/*La propia función main es un thread*/
	thmain = pthread_self();
	/*Inicializa los parámetros de los threads por defecto*/
	//pthread_attr_init(&attr);
	printf("Soy la función MAIN y voy a lanzar el thread con parámetos: %d, %d\n", param.dato1, param.dato2);
	pthread_create(&thread1, NULL, func1, &param);
	pthread_join(thread1, NULL);
	printf("Soy main: he lanzado el thread y he terminado\n");
	pthread_exit(NULL);
}
