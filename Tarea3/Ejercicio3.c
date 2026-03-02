#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define N 1000
#define numProcesos 5

int process_fork(int nProcesos){
	for(int i = 1; i < nProcesos; i++)
		if(fork() == 0)
			return i;
	return 0;
}
int find(int *arre, int lim_inf, int lim_sup, int target){
	for(int i = lim_inf; i <= lim_sup; i++)
		if(arre[i] == target)
			return i;

	return -1;
}



int main (){
	int pid;
	static int arre[N];
	srand((unsigned)time(NULL));
	
	for(int i = 0; i < N; i++)
		arre[i] = rand() % 1000;
	int nb,
	index;
	printf("Ingrese el número a buscar: ");
	scanf("%d", &nb);
	
	pid = process_fork(numProcesos);
	
	switch(pid)
	{
		case 0: 
			for(int j = 1; j < numProcesos; j++)
				wait(&j);
			printf("\n\nPrograma finalizado...\n");
			exit(0);
		case 1:
			printf("\n\nRango [0,..,249]\n");
			index = find(arre, 0, 249, nb);
			if(index != -1)
				printf("El número %d se encontró en la posición [%d]\n", arre[index], index);
			else
				printf("El número no se encontró\n");
			exit(0);
		case 2:
			printf("\n\nRango [250,..,499]\n");
			index = find(arre, 250, 499, nb);
			if(index != -1)
				printf("El número %d se encontró en la posición [%d]\n", arre[index], index);
			else
				printf("El número no se encontró\n");
			exit(0);
		case 3:
			printf("\n\nRango [500,..,749]\n");
			index = find(arre, 500, 749, nb);
			if(index != -1)
				printf("El número %d se encontró en la posición [%d]\n", arre[index], index);
			else
				printf("El número no se encontró\n");
			exit(0);
		case 4:
			printf("\n\nRango [750,..,999]\n");
			index = find(arre, 750, 999, nb);
			if(index != -1)
				printf("El número %d se encontró en la posición [%d]\n", arre[index], index);
			else
				printf("El número no se encontró\n");
			exit(0);
	}



	return 0;
}
