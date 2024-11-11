/************************************************************************************************
Fecha: 31/10/2024
Autor: Daniel Castro - Eliana Cepeda - Maria Paula Rodriguez - Daniel Gonzalez
Materia: Sistemas Operativos
Tema: Taller 6 - Evaluación de rendimiento
Descripción:
	Este programa realiza la multiplicación de matrices cuadradas N×NN×N empleando el modelo
	de hilos POSIX (Pthreads) y evalúa el tiempo de ejecución del algoritmo clásico de 
	multiplicación de matrices distribuyendo el trabajo en múltiples hilos
************************************************************************************************/

#include <stdio.h> // Para entradas y salidas estándar
#include <pthread.h> // Para manejo de hilos y sincronización
#include <unistd.h> // Para llamadas al sistema
#include <stdlib.h> // Para uso de funciones básicas
#include <time.h> // Para manejo de tiempo
#include <sys/time.h> // Para medir tiempo de ejecución

#define DATA_SIZE (1024*1024*64*3) // Tamaño de memoria asignada

pthread_mutex_t MM_mutex; // Mutex para sincronización de hilos
static double MEM_CHUNK[DATA_SIZE]; // Memoria estática para las matrices
double *mA, *mB, *mC; // Punteros a matrices

struct parametros{ // Estructura para parámetros de hilos
	int nH; // Número de hilos
	int idH; // ID del hilo
	int N; // Tamaño de la matriz
};

struct timeval start, stop; // Estructura para medir tiempo

void llenar_matriz(int SZ){ // Función para llenar las matrices
	srand48(time(NULL)); // Inicialización del generador de números aleatorios
	for(int i = 0; i < SZ*SZ; i++){
			mA[i] = 1.1*i; // Valor inicial en matriz A
			mB[i] = 2.2*i; // Valor inicial en matriz B
			mC[i] = 0; // Inicialización de matriz C
		}	
}

void print_matrix(int sz, double *matriz){ // Función para imprimir matrices
	if(sz < 12){ // Condición para imprimir matrices pequeñas
    		for(int i = 0; i < sz*sz; i++){
     				if(i%sz==0) printf("\n"); // Salto de línea por fila
            		printf(" %.3f ", matriz[i]); // Formato de impresión
			}	
    	printf("\n>-------------------->\n"); // Separador de matrices
	}
}

void inicial_tiempo(){ // Iniciar cronómetro
	gettimeofday(&start, NULL); // Guardar tiempo inicial
}

void final_tiempo(){ // Terminar cronómetro
	gettimeofday(&stop, NULL); // Guardar tiempo final
	stop.tv_sec -= start.tv_sec; // Calcular diferencia de segundos
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec)); // Imprimir tiempo
}

void *mult_thread(void *variables){ // Función de multiplicación de matrices en hilos
	struct parametros *data = (struct parametros *)variables; // Cast a estructura de parámetros
	
	int idH = data->idH; // ID del hilo
	int nH  = data->nH; // Número total de hilos
	int N   = data->N; // Tamaño de la matriz
	int ini = (N/nH)*idH; // Índice inicial de multiplicación
	int fin = (N/nH)*(idH+1); // Índice final de multiplicación

    for (int i = ini; i < fin; i++){ // Bucle de filas
        for (int j = 0; j < N; j++){ // Bucle de columnas
			double *pA, *pB, sumaTemp = 0.0; // Punteros y variable temporal
			pA = mA + (i*N); // Posición en matriz A
			pB = mB + j; // Posición en matriz B
            for (int k = 0; k < N; k++, pA++, pB+=N){ // Bucle de acumulación
				sumaTemp += (*pA * *pB); // Suma temporal del producto
			}
			mC[i*N+j] = sumaTemp; // Asignación en matriz C
		}
	}

	pthread_mutex_lock (&MM_mutex); // Bloquear el mutex
	pthread_mutex_unlock (&MM_mutex); // Desbloquear el mutex
	pthread_exit(NULL); // Terminar hilo
}

int main(int argc, char *argv[]){ // Función principal
	if (argc < 2){ // Verificación de argumentos
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n"); // Instrucción de uso
		return -1;	// Retorno en caso de error
	}
    int SZ = atoi(argv[1]); // Conversión del tamaño de matriz
    int n_threads = atoi(argv[2]); // Conversión del número de hilos

    pthread_t p[n_threads]; // Arreglo de identificadores de hilos
    pthread_attr_t atrMM; // Atributos de hilo

	mA = MEM_CHUNK; // Asignación de memoria para matriz A
	mB = mA + SZ*SZ; // Asignación de memoria para matriz B
	mC = mB + SZ*SZ; // Asignación de memoria para matriz C

	llenar_matriz(SZ); // Llenar matrices A y B
	print_matrix(SZ, mA); // Imprimir matriz A
	print_matrix(SZ, mB); // Imprimir matriz B

	inicial_tiempo(); // Iniciar cronómetro
	pthread_mutex_init(&MM_mutex, NULL); // Inicializar mutex
	pthread_attr_init(&atrMM); // Inicializar atributos
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE); // Hilos unibles

    for (int j=0; j<n_threads; j++){ // Creación de hilos
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); // Reserva de memoria para parámetros
		datos->idH = j; // Asignación de ID de hilo
		datos->nH  = n_threads; // Número total de hilos
		datos->N   = SZ; // Tamaño de la matriz
        pthread_create(&p[j],&atrMM,mult_thread,(void *)datos); // Creación de hilo
	}

    for (int j=0; j<n_threads; j++) // Espera a que finalicen los hilos
        pthread_join(p[j],NULL); // Unir hilos al hilo principal
	final_tiempo(); // Terminar cronómetro
	
	print_matrix(SZ, mC); // Imprimir matriz C

	pthread_attr_destroy(&atrMM); // Destruir atributos de hilo
	pthread_mutex_destroy(&MM_mutex); // Destruir mutex
	pthread_exit (NULL); // Terminar programa
}