#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define N 10000
#define numProcesos 4

int process_fork(int nProcesos){
	for(int i = 1; i < nProcesos; i++)
		if(fork() == 0)
			return i;
	return 0;
}
int suma_total(int *arre, int ultimaPosicion){
	int total = 0;
	for(int i = 0; i < ultimaPosicion; i++)
		total = total + arre[i];//19
	return total;
}
int la_moda(int *arre, int ultimaPosicion){
	int moda = arre[0];
	int maxFrecuencia = 0;
	
	for(int i = 0; i < ultimaPosicion; i++){
		int cont = 0;
		for(int j = 0; j < ultimaPosicion; j++)
			if(arre[j] == arre[i])
				cont++;
		if(cont > maxFrecuencia){
			maxFrecuencia = cont;
			moda = arre[i];
		}
	}
	return moda;
}
int compararEnteros(const void *a, const void *b){
	return (*(int*)a - *(int*)b);
}
/*
void escribirArch(int num){
	FILE *fp;
	if((fp = fopen("arreglo.txt", "a")) == NULL)
		printf("Error al abrir el archivo\n");
	else
	{
		fprintf(fp, "%d\n", num);
		fclose(fp);
	}
}
*/
int find(int *arre, int size, int target){
	for(int i = 0; i < size; i++)
		if(arre[i] == target)
			return i;
	return -1;


}
int main(){
	int pid;
	static int arre[N];
	srand((unsigned)time(NULL));
	
	for(int i = 0; i < N; i++)
		arre[i] = rand() % 10000;
	
	pid = process_fork(numProcesos);
	
	switch(pid)
	{
		case 0:
			for(int k = 1; k < numProcesos; k++)
				wait(&k);
			int nbuscar,
			index;

			printf("Ingrese el número a buscar: ");
			scanf("%d", &nbuscar);
			index = find(arre, N, nbuscar);
			if(index != -1)
				printf("Se encontró el elemento %d en la posición %d\n", arre[index], index);
			else
				printf("No se encontró el elemento\n");
			exit(0);
			//Proceso padre (último proceso porque lleva barrera);
		case 1:
			int total = suma_total(arre, N);
			printf("La suma total es: %d\n\n\n", total);
			exit(0);
		case 2:
			int moda = la_moda(arre, N);
			printf("La moda es: %d\n\n\n", moda);
			exit(0);
		case 3:
			int n = sizeof(arre) / sizeof(arre[0]);
			/*---- Opción para escribir en archivo.txt ----
			for(int i = 0; i < N; i++)
				escribirArch(arre[i]);*/

			//Opción para imprimir en consola
			printf("Vector original:\n");
			for(int i = 0; i < 10; i++)
				printf("%d\n", arre[i]);
			printf("...arre[9999]\n");
			qsort(arre, n, sizeof(int), compararEnteros);
			
			/*escribirArch(1);
			escribirArch(1);
			for(int i = 0; i < N; i++)
				escribirArch(arre[i]);*/
			
			//printf("El arreglo se ha ordenado y se ha escrito en el archivo...\n");

			//Opción para imprimir en consola
			printf("El arreglo se está ordenando...\n");
			//sleep(2);
			printf("Vector ordenador:\n");
			for(int j = 0; j < 10; j++)
				printf("%d\n", arre[j]);
			printf("...arre[9999]\n\n\n");
			exit(0);
			
	}

	return 0;
}
