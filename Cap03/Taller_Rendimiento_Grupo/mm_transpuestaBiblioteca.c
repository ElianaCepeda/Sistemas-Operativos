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
#include <stdio.h>               // Incluye funciones estándar de entrada/salida
#include <pthread.h>             // Incluye la biblioteca de hilos POSIX
#include <unistd.h>              // Incluye funciones POSIX, como sleep y usleep
#include <stdlib.h>              // Incluye funciones estándar de C para gestión de memoria
#include <time.h>                // Incluye funciones para manejo de tiempo
#include <sys/time.h>            // Incluye estructuras y funciones para medir tiempos en microsegundos
#include "mm_transpuestaBiblioteca.h"  // Incluye el archivo de cabecera para usar variables y funciones compartidas

pthread_mutex_t MM_mutex;        // Mutex para sincronización en acceso a la memoria compartida
double MEM_CHUNK[DATA_SIZE];     // Bloque de memoria grande para almacenar las matrices
double *mA, *mB, *mC;            // Punteros a matrices A, B, y C, asignados dentro de `MEM_CHUNK`
struct timeval start, stop;      // Estructuras para medir el tiempo de inicio y fin de ejecución

void llenar_matriz(int SZ){ 
	srand48(time(NULL));         // Inicializa la semilla para generación de números aleatorios
	for(int i = 0; i < SZ*SZ; i++){  // Recorre todos los elementos de las matrices
		mA[i] = 1.1 * i;            // Asigna valores secuenciales a `mA` (por ejemplo, 1.1 * i)
		mB[i] = 2.2 * i;            // Asigna valores secuenciales a `mB` (por ejemplo, 2.2 * i)
		mC[i] = 0;                  // Inicializa todos los elementos de `mC` a 0
	}	
}

void print_matrix(int sz, double *matriz){
	if(sz < 12){                 // Imprime la matriz solo si su tamaño es pequeño (menor a 12x12)
		for(int i = 0; i < sz*sz; i++){   // Recorre todos los elementos de la matriz
			if(i % sz == 0) printf("\n");  // Salto de línea al final de cada fila
			printf(" %.3f ", matriz[i]);   // Imprime cada elemento de la matriz con 3 decimales
		}	
		printf("\n>-------------------->\n");  // Línea divisoria tras imprimir la matriz
	}
}

void inicial_tiempo(){
	gettimeofday(&start, NULL);  // Almacena el tiempo de inicio en la variable `start`
}

void final_tiempo(){
	gettimeofday(&stop, NULL);   // Almacena el tiempo de finalización en la variable `stop`
	stop.tv_sec -= start.tv_sec; // Calcula la diferencia de segundos entre `stop` y `start`
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec));  // Muestra el tiempo transcurrido en microsegundos
}

void *mult_thread(void *variables){
	struct parametros *data = (struct parametros *)variables; // Convierte el argumento `variables` a tipo `parametros`
	
	int idH = data->idH;         // Almacena el ID del hilo
	int nH  = data->nH;          // Almacena el número total de hilos
	int N   = data->N;           // Almacena el tamaño de la matriz
	int ini = (N / nH) * idH;    // Calcula la fila inicial para este hilo
	int fin = (N / nH) * (idH + 1); // Calcula la fila final para este hilo

    for (int i = ini; i < fin; i++){    // Recorre las filas asignadas al hilo
        for (int j = 0; j < N; j++){    // Recorre cada columna de la matriz
			double *pA, *pB, sumaTemp = 0.0; // Declara punteros para recorrer `mA` y `mB`, e inicializa `sumaTemp`
			pA = mA + (i * N);          // Puntero al inicio de la fila `i` en `mA`
			pB = mB + (j * N);          // Puntero al inicio de la columna `j` en `mB`
            for (int k = 0; k < N; k++, pA++, pB++){ // Recorre los elementos de la fila y columna
				sumaTemp += (*pA * *pB); // Acumula el producto de los elementos de `mA` y `mB` en `sumaTemp`
			}
			mC[i * N + j] = sumaTemp;   // Asigna `sumaTemp` a la posición correspondiente en `mC`
		}
	}

	pthread_mutex_lock(&MM_mutex); // Bloquea el mutex para sincronización
	pthread_mutex_unlock(&MM_mutex); // Desbloquea el mutex
	pthread_exit(NULL);            // Termina el hilo
}
