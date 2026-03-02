#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>


#define procesos 3

int process_fork(int),
nP(), 
numero();

int main(){
	int pid;
	int pipes[procesos][2];
	for(int i = 0; i < procesos; i++)
		pipe(pipes[i]);

	
	pid = process_fork(procesos);
	switch(pid)
	{
		case 0:
			//Código padre
			int n;
			for(int i = 0; i < procesos; i++)
				close(pipes[i][0]);
			for(;;){
				n = numero();
				write(pipes[nP()][1],&n,sizeof(int));
			}
			for(int k = 0; k < procesos; k++)
				wait(&k);
			exit(0);
		case 1:
			//Hijo1
			int arre[4], cont = 0, num;
			for(int i = 0; i < procesos; i++)
				if(i == pid - 1)
					close(pipes[i][1]);
				else{
					close(pipes[i][0]);
					close(pipes[i][1]);
				}

			while(cont < 4 && (read(pipes[0][0], &num, sizeof(int))) > 0)
				arre[cont++] = num;
			if(cont == 4){
				printf("Gané[PROCESO: %d]: ", pid);
				printf("[ ");
				for(int t = 0; t < 4; t++)
					printf("%d  ", arre[t]);
				printf("]\n\n");
				kill(0, SIGTERM);
			}
			exit(0);
		case 2: 
			//Hijo2
			int arre2[4], cont2 = 0, num2;
			for(int i = 0; i < procesos; i++)
				if(i == pid - 1)
					close(pipes[i][1]);
				else{
					close(pipes[i][0]);
					close(pipes[i][1]);
				}
			while(cont2 < 4 && (read(pipes[1][0], &num2, sizeof(int))) > 0)
				arre2[cont2++] = num2;
			if(cont2 == 4){
				printf("Gané[PROCESO: %d]: ", pid);
				printf("[ ");
				for(int t = 0; t < 4; t++)
					printf("%d  ", arre2[t]);
				printf("]\n\n");
				kill(0, SIGTERM);
			}
			exit(0);
		case 3: 
			//Hijo3
			int arre3[4], cont3 = 0, num3;
			for(int i = 0; i < procesos; i++)
				if(i == pid - 1)
					close(pipes[i][1]);
				else{
					close(pipes[i][0]);
					close(pipes[i][1]);
				}
			while(cont3 < 4 && (read(pipes[2][0], &num3, sizeof(int))) > 0)
				arre3[cont3++] = num3;
			if(cont3 == 4){
				printf("Gané[PROCESO: %d]: ", pid);
				printf("[ ");
				for(int t = 0; t < 4; t++)
					printf("%d  ", arre3[t]);
				printf("]\n\n");
				kill(0, SIGTERM);
			}
			exit(0);
	}

	return 0;
}
int process_fork(int nProcesos){
	for(int i = 1; i <= nProcesos; i++)
		if(fork() == 0)
			return i;
	return 0;
}

int nP(){
	int proceso = 0;
	proceso = rand() % 3;
	return proceso;
}
int numero(){
	int num = 0;
	num = rand() % 21;
	return num;
}
