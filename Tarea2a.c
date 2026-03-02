#include <stdio.h> 
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define N 10000
#define numProcesos 6

int process_fork(int nProcesos){
	for(int i = 1; i < nProcesos; i++)
		if(fork() == 0)
			return i;
	return 0;
}
int max(int *arre, int lim_inf, int lim_sup){
	int max = arre[lim_inf];
	for(int i = (lim_inf + 1); i <= lim_sup; i++)
		if(arre[i] > max)
			max = arre[i];
	return max;
}

void escribirMayor(int mayor){
	
	FILE *fp;

	if((fp = fopen("min.txt", "a")) == NULL)
		printf("Error al abrir el archivo");
	else
	{
		fprintf(fp, "%d\n", mayor);
		fclose(fp);
	}
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
		case 0:printf("Estoy en proceso\n");
			int buf, men = 10000;
			
			/*
			for(int j = 0; j < numProcesos; j++)
				wait(&j);
			/*
			int status;
			while(1){
				pid_t r = wait(&status);
				if(r > 0)
					continue;
				if(r == -1 && errno == ECHILD)
					break;
			}*/
			FILE *fp;
			if((fp = fopen("min.txt", "r")) == NULL)
				printf("Error al abrir el archivo\n");
			else{
				printf("Buscando archivo...\n");
				while(!feof(fp)){
				fscanf(fp, "%d", &buf);
				if(buf < men)	
					men = buf;
				}
				fclose(fp);
			}


			printf("Soy el proceso padre[PID: %d], y el menor de mayores es: %d\n",getpid(), men);
			exit(0);
		case 1:
			int msx = max(arre, 0, 1999);
			printf("Proceso: %d --- max(A[0]...A[1,999]) = %d\n", pid,msx);
			escribirMayor(msx);
			exit(0);
		case 2: 
			int msx2 = max(arre, 2000, 3999);
			printf("Proceso: %d --- max(A[2,000]...A[3,999]) = %d\n", pid, msx2);
			escribirMayor(msx2);
			exit(0);
		case 3:
			int msx3 = max(arre, 4000, 5999);
			printf("Proceso: %d --- max(A[4,000]...A[5,999]) = %d\n", pid, msx3);
			escribirMayor(msx3);
			exit(0);
		case 4: 
			int msx4 = max(arre, 6000, 7999);
			printf("Proceso: %d --- max(A[6,000]...A[7,999]) = %d\n", pid, msx4);
			escribirMayor(msx4);
			exit(0);
		case 5:
			int msx5 = max(arre, 8000, 9999);
			printf("Proceso: %d --- max(A[8,000]...A[9,999]) = %d\n", pid,msx5);
			escribirMayor(msx5);
			exit(0);
	}
	return 0;
}
