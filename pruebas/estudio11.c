#include <stdio.h> 
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define N 10000
#define numProcesos 5

int process_fork(int nProcesos){
	for(int i = 1; i < nProcesos; i++)
		if(fork() == 0)
			return i;
	return 0;
}
int max(int *arre, int lim_inf, int lim_sup){
	int max = lim_inf;
	for(int i = (lim_inf + 1); i <= lim_sup; i++)
		if(arre[i] > max)
			max = arre[i];
	return max;
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
			int status;
			while(1){
				pid_t r = wait(&status);
				if(r > 0)
					continue;
				if(r == -1 && errno == ECHILD)
					break;
			}
			printf("[PROCESO PADRE] Proceso: %d --- max(A[8,000]...A[9,999]) = %d\n",(pid + 5), max(arre, 8000,9999 ));
			exit(0);
		case 1:printf("Proceso: %d --- max(A[0]...A[1,999]) = %d\n", pid,max(arre, 0,1999));
			exit(0);
		case 2: printf("Proceso: %d --- max(A[2,000]...A[3,999]) = %d\n", pid, max(arre, 2000,3999));
			exit(0);
		case 3: printf("Proceso: %d --- max(A[4,000]...A[5,999]) = %d\n", pid, max(arre, 4000,5999));
			exit(0);
		case 4: printf("Proceso: %d --- max(A[6,000]...A[7,999]) = %d\n", pid, max(arre, 6000,7999));
			exit(0);
	}
	return 0;
}
