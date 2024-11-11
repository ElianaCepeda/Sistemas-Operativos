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
#ifndef MM_CLASICOBIBLIOTECA_H
#define MM_CLASICOBIBLIOTECA_H

#include <stdio.h>              // Incluye funciones de entrada y salida estándar
#include <pthread.h>            // Incluye la biblioteca para manejo de hilos POSIX
#include <unistd.h>             // Incluye funciones de POSIX para manipulación de procesos y hilos
#include <stdlib.h>             // Incluye funciones de gestión de memoria dinámica y otros procesos
#include <time.h>               // Incluye funciones para manejo de tiempo
#include <sys/time.h>           // Incluye estructuras y funciones para medir tiempo a nivel de microsegundos

#define DATA_SIZE (1024*1024*64*3)  // Define el tamaño total de la memoria para matrices en `MEM_CHUNK`

// Declaración de variables globales como `extern`
extern pthread_mutex_t MM_mutex;  // Mutex global para sincronización de hilos
extern double *mA, *mB, *mC;      // Punteros globales a matrices A, B, y C en `MEM_CHUNK`
extern double MEM_CHUNK[DATA_SIZE];  // Espacio de memoria compartido para almacenar matrices
extern struct timeval start, stop;   // Variables de tiempo para medir duración de la operación

// Estructura que contiene parámetros necesarios para cada hilo
struct parametros{
	int nH;  // Número total de hilos a utilizar
	int idH; // Identificador único del hilo
	int N;   // Dimensión de la matriz (N x N)
};

// Declaraciones de funciones que implementarán el cálculo y manejo de matrices

// Función que llena matrices A y B con valores iniciales y limpia la matriz C
void llenar_matriz(int SZ);

// Función que imprime una matriz en la consola si su tamaño es menor a 12x12
void print_matrix(int sz, double *matriz);

// Función que inicia el temporizador para medir el tiempo de ejecución
void inicial_tiempo();

// Función que detiene el temporizador y muestra el tiempo transcurrido
void final_tiempo();

// Función de multiplicación de matrices, ejecutada por cada hilo
void *mult_thread(void *variables);

#endif  // Cierre de la instrucción de preprocesador para evitar definiciones múltiples
