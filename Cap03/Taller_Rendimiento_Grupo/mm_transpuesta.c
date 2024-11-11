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
#include <unistd.h>              // Incluye funciones de POSIX, como sleep y usleep
#include <stdlib.h>              // Incluye funciones estándar de C para gestión de memoria y procesos
#include <time.h>                // Incluye funciones para manejar el tiempo
#include <sys/time.h>            // Incluye estructuras y funciones para tiempos a nivel de microsegundos
#include "mm_transpuestaBiblioteca.h"  // Incluye el archivo de cabecera con funciones auxiliares y variables globales

// Función principal (main) del programa de multiplicación de matrices
int main(int argc, char *argv[]){
	// Verifica que se hayan pasado los argumentos necesarios (tamaño de la matriz y número de hilos)
	if (argc < 2){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n"); // Mensaje de error si faltan argumentos
		return -1;	 // Termina el programa con código de error
	}

    int SZ = atoi(argv[1]);       // Convierte el primer argumento a entero, representando el tamaño de la matriz
    int n_threads = atoi(argv[2]); // Convierte el segundo argumento a entero, representando el número de hilos a utilizar

    pthread_t p[n_threads];        // Arreglo de hilos de tamaño `n_threads`
    pthread_attr_t atrMM;          // Declaración de atributos para los hilos

	// Inicializa los punteros globales de matrices mA, mB, y mC en el bloque de memoria `MEM_CHUNK`
	mA = MEM_CHUNK;                // Asigna la posición inicial de `MEM_CHUNK` a mA
	mB = mA + SZ*SZ;               // Coloca `mB` justo después de `mA` en la memoria
	mC = mB + SZ*SZ;               // Coloca `mC` justo después de `mB` en la memoria

	// Llama a la función para llenar las matrices `mA` y `mB` y prepara `mC`
	llenar_matriz(SZ);
	// Imprime `mA` y `mB` en consola para verificación (si el tamaño es pequeño)
	print_matrix(SZ, mA);
	print_matrix(SZ, mB);

	// Inicia el temporizador para medir el tiempo de ejecución
	inicial_tiempo();
	pthread_mutex_init(&MM_mutex, NULL);     // Inicializa el mutex para sincronización de hilos
	pthread_attr_init(&atrMM);               // Inicializa los atributos de los hilos
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);  // Configura los hilos como "joinable" para esperar su finalización

    // Bucle para crear `n_threads` hilos, cada uno ejecutando `mult_thread`
    for (int j=0; j<n_threads; j++){
		// Reserva memoria para la estructura `parametros` que contiene datos específicos para cada hilo
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		datos->idH = j;          // Asigna el ID del hilo actual
		datos->nH  = n_threads;  // Guarda el número total de hilos
		datos->N   = SZ;         // Guarda el tamaño de la matriz

        // Crea un hilo que ejecuta `mult_thread` y le pasa `datos` como argumento
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos);
	}

    // Espera a que todos los hilos terminen su ejecución
    for (int j=0; j<n_threads; j++)
        pthread_join(p[j], NULL);  // Espera a que el hilo `p[j]` finalice
	final_tiempo();               // Termina el temporizador y muestra el tiempo de ejecución
	
	// Imprime la matriz `mC` resultante en consola para verificación (si el tamaño es pequeño)
	print_matrix(SZ, mC);

	pthread_attr_destroy(&atrMM);   // Libera los recursos de los atributos de los hilos
	pthread_mutex_destroy(&MM_mutex);  // Destruye el mutex `MM_mutex`
	pthread_exit(NULL);            // Termina el hilo principal y libera recursos
}
