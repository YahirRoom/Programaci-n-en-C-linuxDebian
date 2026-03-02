#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// Definiciones de tamaño: N=4 (matriz 4x4), K=2 (bloque 2x2)
#define N 4
#define K 2
// --- Matrices Globales ---
// Matriz A (4x4) - Usando los valores del ejemplo del PDF (corrigiendo la fila 2incompleta) 
int A[N][N] = {  
{1, 0, 2, 3}, 
{4, -1, 1, 5}, 
{-2, -3, -4, 2}, 
{-1, 2, 0, 0} 
};
// Matriz B (4x4) - Usando los valores del ejemplo del PDF (completando los valoresfaltantes para ser 4x4) 
int B[N][N] = {  
{-1, 1, 2, -3}, 
{-5, -4, 2, -2}, 
{3, -1, 0, 2}, 
{1, 0, 4, 5} 
};
int C[N][N] = {0}; // Matriz resultado C, inicializada a cero
// Estructura para pasar parámetros a cada hilo
typedef struct
{
	int i; // Índice de fila del bloque de C (0 o 1, para C_1x o C_2x)
	int j; // Índice de columna del bloque de C (0 o 1, para C_x1 o C_x2)
} block_indices;
// --- Función de Multiplicación de Bloques (Ejecutada por cada Hilo) ---
void *multiply_block(void *arg)
{
	block_indices *indices = (block_indices *)arg;
	int I = indices->i; // Fila del bloque (0 o 1)
	int J = indices->j; // Columna del bloque (0 o 1)
	// Coordenadas de inicio del bloque en la matriz C
	int start_row = I * K;
	int start_col = J * K;
	// Fórmula Correcta: C_IJ = SUM(A_IL * B_LJ) para L=0 a 1
	// Multiplicación de la primera submatriz (L=0) y la segunda (L=1)
	for (int L = 0; L < 2; L++)
	{
		// Multiplica la submatriz A_IL por B_LJ y suma a C_IJ
		for (int r = 0; r < K; r++)
		{ // Fila dentro del bloque
			for (int c = 0; c < K; c++)
			{ // Columna dentro del bloque
				int sum = 0;
				for (int k = 0; k < K; k++)
				{ // Cálculo del producto punto
					// Elemento A_IL [I*K + r][L*K + k]
					// Elemento B_LJ [L*K + k][J*K + c]
					sum += A[I * K + r][L * K + k] * B[L * K + k][J * K + c];
				}
				// Acumula el resultado en C_IJ
				C[start_row + r][start_col + c] += sum;
			}
		}
	}
	printf("Hilo C_%d%d: Cálculo del bloque terminado.\n", I + 1, J + 1);
	pthread_exit(NULL);
}
// --- Función para imprimir la matriz ---
void print_matrix(int M[N][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%4d ", M[i][j]);
		}
		printf("\n");
	}
}
// --- Función Principal ---
int main()
{
	pthread_t threads[4];
	block_indices indices[4];
	// Definir los índices para los 4 bloques: C_11, C_12, C_21, C_22
	indices[0] = (block_indices){0, 0}; // C_11
	indices[1] = (block_indices){0, 1}; // C_12
	indices[2] = (block_indices){1, 0}; // C_21
	indices[3] = (block_indices){1, 1}; // C_22
	printf("--- Multiplicación de Matrices por Bloques (4 Hilos) ---\n");
	printf("Matriz A (4x4):\n");
	print_matrix(A);
	printf("Matriz B (4x4):\n");
	print_matrix(B);
	printf("------------------------------------------------------\n");
	// Crear 4 hilos para calcular cada bloque
	for (int i = 0; i < 4; i++)
	{
		pthread_create(&threads[i], NULL, multiply_block, &indices[i]);
	}
	// Esperar a que todos los hilos terminen
	for (int i = 0; i < 4; i++)
	{
		pthread_join(threads[i], NULL);
	}
	printf("------------------------------------------------------\n");
	printf("Matriz Resultado C:\n");
	print_matrix(C);
	return 0;
}
