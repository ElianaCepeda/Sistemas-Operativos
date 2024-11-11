/**************************************************************
		Pontificia Universidad Javeriana
	Autor: Daniel Castro, Maria Ruiz, Daniel Gonzalez, Eliana Cepeda
	Fecha: Noviembre 2024
	Materia: Sistemas Operativos
	Tema: Taller de Evaluación de Rendimiento
	Fichero: fuente de multiplicación de matrices NxN por hilos.
	Objetivo: Evaluar el tiempo de ejecución del 
		      algoritmo clásico de multiplicación de matrices.
			  Se implementa con la Biblioteca POSIX Pthreads
****************************************************************/

#include <stdio.h>             // Biblioteca estándar de entrada/salida
#include <pthread.h>           // Biblioteca para trabajar con hilos POSIX
#include <unistd.h>            // Biblioteca para operaciones de sistema
#include <stdlib.h>            // Biblioteca para manejo de memoria dinámica
#include <time.h>              // Biblioteca para manejo de operaciones de tiempo
#include <sys/time.h>          // Biblioteca para operaciones de tiempo de alta precisión
#include "mm_clasicoBiblioteca.h" // Header personalizado con declaraciones de funciones y variables

pthread_mutex_t MM_mutex;      // Declaración del mutex para control de secciones críticas
double MEM_CHUNK[DATA_SIZE];   // Arreglo de memoria compartida para almacenar matrices
double *mA, *mB, *mC;          // Punteros a matrices
struct timeval start, stop;    // Estructuras para medir tiempo de ejecución

void llenar_matriz(int SZ) {    // Llena matrices mA y mB con valores iniciales, y asigna cero a mC
	srand48(time(NULL));        // Inicializa la semilla para generación de números aleatorios
	for(int i = 0; i < SZ * SZ; i++) { // Itera sobre cada elemento de las matrices
			mA[i] = 1.1 * i;    // Asigna valores secuenciales a mA
			mB[i] = 2.2 * i;    // Asigna valores secuenciales a mB
			mC[i] = 0;          // Inicializa mC a cero
	}	
}

void print_matrix(int sz, double *matriz) { // Imprime la matriz en consola si es menor a 12x12
	if(sz < 12) {              // Solo imprime si el tamaño es menor a 12 para evitar sobrecarga en consola
		for(int i = 0; i < sz * sz; i++) { // Itera sobre cada elemento de la matriz
			if(i % sz == 0) printf("\n"); // Salto de línea al final de cada fila
			printf(" %.3f ", matriz[i]);  // Imprime el valor de cada elemento con tres decimales
		}	
		printf("\n>-------------------->\n"); // Línea separadora
	}
}

void inicial_tiempo() {         // Inicia la medición de tiempo
	gettimeofday(&start, NULL); // Captura el tiempo actual en `start`
}

void final_tiempo() {           // Finaliza la medición de tiempo y calcula el tiempo transcurrido
	gettimeofday(&stop, NULL);  // Captura el tiempo actual en `stop`
	stop.tv_sec -= start.tv_sec; // Calcula segundos transcurridos
	printf("\n:-> %9.0f µs\n", (double)(stop.tv_sec * 1000000 + stop.tv_usec)); // Imprime tiempo en microsegundos
}

void *mult_thread(void *variables) { // Función para multiplicar matrices en secciones, ejecutada por cada hilo
	struct parametros *data = (struct parametros *)variables; // Castea y obtiene parámetros

	int idH = data->idH;        // ID del hilo actual
	int nH = data->nH;          // Número total de hilos
	int N = data->N;            // Tamaño de la matriz
	int ini = (N / nH) * idH;   // Índice de fila inicial para este hilo
	int fin = (N / nH) * (idH + 1); // Índice de fila final para este hilo

    for (int i = ini; i < fin; i++) { // Bucle sobre las filas asignadas a este hilo
        for (int j = 0; j < N; j++) { // Bucle sobre columnas
			double *pA, *pB, sumaTemp = 0.0; // Variables auxiliares para multiplicación
			pA = mA + (i * N);      // Apunta a la fila de mA
			pB = mB + j;            // Apunta a la columna de mB
            for (int k = 0; k < N; k++, pA++, pB += N) { // Multiplica y suma en sumaTemp
				sumaTemp += (*pA * *pB);
			}
			mC[i * N + j] = sumaTemp; // Asigna resultado a mC
		}
	}

	pthread_mutex_lock(&MM_mutex);  // Bloquea el mutex para proteger secciones críticas
	pthread_mutex_unlock(&MM_mutex); // Desbloquea el mutex
	pthread_exit(NULL);             // Termina el hilo actual
}
