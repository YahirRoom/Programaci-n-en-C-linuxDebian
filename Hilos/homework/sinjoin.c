#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#define precio 20

/*
typedef struct
{
	int *pag;
}caja;
*/

//Variables globales
int cantProducto = 5, pago = 0, cambio = 0;

bool hayProducto(int nProducto);
void *recibirDinero(void *);
void *entregarProducto(void *);

pthread_attr_t attr;
pthread_t thread1, thread2, thmain;
pthread_mutex_t exmut;

bool hayProducto(int nProducto)
{
	return nProducto > 0;
}
void *recibirDinero(void *arg)
{
	int pago1 = 0;
	printf("Ingrese su dinero: ");
	scanf("%d", &pago1);
	if(pago1 >= precio)
	{
		pthread_mutex_lock(&exmut);
		pago = pago1;
		if(pago1 > precio)
			cambio = pago1 - precio;
		pthread_mutex_unlock(&exmut);
	}else
		printf("Dinero insuficiente...devolviendo %d\n", pago1);
	
	pthread_exit(NULL);
}
void *entregarProducto(void *arg)
{
	if(hayProducto(cantProducto))
	{
		pthread_mutex_lock(&exmut);
		if(pago >= precio)
		{
			printf("Soltando producto...\n");
			pago = 0;
			cantProducto--;
			if(cambio > 0)
				printf("Cambio: %d\n", cambio);
				cambio = 0;
		}
		pthread_mutex_unlock(&exmut);
	}else
	{
		pthread_mutex_lock(&exmut);
		printf("Devolviendo dinero (%d), no hay producto...\n");
		pthread_mutex_unlock(&exmut);
	}
	pthread_exit(NULL);
}

int main()
{
	thmain = pthread_self();
	pthread_mutex_init(&exmut, NULL);
	pthread_attr_init(&attr);
	
	
	printf("-----MÁQUINA EXPENDEDORA------\n");

	pthread_mutex_lock(&exmut);
	printf("Cantidad de productos: %d\n", cantProducto);
	pthread_mutex_unlock(&exmut);
	pthread_create(&thread1, &attr, recibirDinero, NULL);
	//pthread_join(thread1, NULL);
	pthread_create(&thread2, &attr, entregarProducto, NULL);
	//pthread_join(thread2, NULL);
	pthread_mutex_lock(&exmut);
	if(cantProducto == 0)
	{
		printf("El producto se ha terminado\n");
		printf("¡Gracias por usar la máquina expendedora!\n");
	}
	pthread_mutex_unlock(&exmut);
	
	pthread_exit(NULL);
}

