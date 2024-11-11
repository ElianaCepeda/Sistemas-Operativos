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
#ifndef MM_TRANSPUESTABIBLIOTECA_H     // Inicio de la protección contra inclusión múltiple del archivo
#define MM_TRANSPUESTABIBLIOTECA_H     // Define el identificador de inclusión única del archivo

#include <stdio.h>                     // Incluye funciones estándar de entrada/salida
#include <pthread.h>                   // Incluye la biblioteca de hilos POSIX
#include <unistd.h>                    // Incluye funciones POSIX como sleep y usleep
#include <stdlib.h>                    // Incluye funciones estándar de C para gestión de memoria y procesos
#include <time.h>                      // Incluye funciones para manejo de tiempo
#include <sys/time.h>                  // Incluye estructuras y funciones para tiempos a nivel de microsegundos

#define DATA_SIZE (1024*1024*64*3)     // Define el tamaño total de la memoria para matrices en `MEM_CHUNK`

extern pthread_mutex_t MM_mutex;       // Declaración de un mutex global para sincronización de hilos
extern double MEM_CHUNK[DATA_SIZE];    // Declaración de un bloque de memoria para almacenar matrices
extern double *mA, *mB, *mC;           // Punteros globales a matrices A, B, y C en `MEM_CHUNK`
extern struct timeval start, stop;     // Variables de tiempo para medir duración de la operación

struct parametros{                     // Estructura para almacenar parámetros necesarios para cada hilo
	int nH;                            // Número total de hilos a utilizar
	int idH;                           // Identificador único del hilo
	int N;                             // Dimensión de la matriz (N x N)
};

// Declaraciones de funciones utilizadas en `mm_transpuestaBiblioteca.c`

void llenar_matriz(int SZ);            // Función para llenar matrices A y B y preparar la matriz C
void print_matrix(int sz, double *matriz); // Función para imprimir una matriz si el tamaño es menor a 12x12
void inicial_tiempo();                 // Función para iniciar el temporizador
void final_tiempo();                   // Función para detener el temporizador y mostrar tiempo transcurrido
void *mult_thread(void *variables);    // Función de multiplicación de matrices, ejecutada por cada hilo

#endif                                 // Fin de la protección contra inclusión múltiple del archivo
