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
#include <stdlib.h>            // Biblioteca estándar de C para manejo de memoria y conversiones
#include <time.h>              // Biblioteca para manejar operaciones de tiempo
#include <sys/time.h>          // Biblioteca para operaciones de tiempo de alta precisión
#include "mm_clasicoBiblioteca.h" // Header personalizado que contiene funciones y variables globales

int main(int argc, char *argv[]) {
	if (argc < 2) {              // Verifica si el número de argumentos es menor a 2
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n"); // Mensaje de error
		return -1;                // Retorna -1 indicando error en ejecución por falta de argumentos
	}
    int SZ = atoi(argv[1]);      // Convierte el primer argumento a entero, representa el tamaño de la matriz
    int n_threads = atoi(argv[2]); // Convierte el segundo argumento a entero, representa el número de hilos

    pthread_t p[n_threads];       // Arreglo para almacenar los identificadores de hilos
    pthread_attr_t atrMM;         // Declaración de atributos de los hilos

	mA = MEM_CHUNK;               // Asigna inicio de MEM_CHUNK a la matriz mA
	mB = mA + SZ*SZ;              // Asigna siguiente bloque de memoria a la matriz mB
	mC = mB + SZ*SZ;              // Asigna siguiente bloque de memoria a la matriz mC

	llenar_matriz(SZ);            // Llama a la función para llenar las matrices mA y mB
	print_matrix(SZ, mA);         // Imprime la matriz mA en consola
	print_matrix(SZ, mB);         // Imprime la matriz mB en consola

	inicial_tiempo();             // Inicia la medición del tiempo de ejecución
	pthread_mutex_init(&MM_mutex, NULL);  // Inicializa el mutex para proteger secciones críticas
	pthread_attr_init(&atrMM);    // Inicializa los atributos de los hilos
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE); // Configura los hilos como joinable

    for (int j = 0; j < n_threads; j++) { // Itera para crear n_threads hilos
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); // Asigna memoria para la estructura de parámetros
		datos->idH = j;           // Asigna el ID del hilo
		datos->nH = n_threads;     // Asigna el número total de hilos
		datos->N = SZ;             // Asigna el tamaño de la matriz
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos); // Crea el hilo y ejecuta mult_thread con datos
	}

    for (int j = 0; j < n_threads; j++) // Itera para unir todos los hilos
        pthread_join(p[j], NULL);       // Espera a que cada hilo termine

	final_tiempo();              // Finaliza la medición de tiempo de ejecución
	
	print_matrix(SZ, mC);        // Imprime la matriz de resultado mC en consola

	pthread_attr_destroy(&atrMM); // Destruye los atributos de hilo
	pthread_mutex_destroy(&MM_mutex); // Destruye el mutex
	pthread_exit(NULL);           // Termina el hilo principal
}
