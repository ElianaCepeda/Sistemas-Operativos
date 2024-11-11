/************************************************************************************************
Fecha: 31/10/2024
Autor: Daniel Castro - Eliana Cepeda - Maria Paula Rodriguez - Daniel Gonzalez
Materia: Sistemas Operativos
Tema: Taller 6 - Evaluación de rendimiento
Descripción:
    Este programa realiza la multiplicación de matrices cuadradas N×N empleando el modelo
    de hilos POSIX (Pthreads) y evalúa el tiempo de ejecución del algoritmo clásico de 
    multiplicación de matrices distribuyendo el trabajo en múltiples hilos
************************************************************************************************/

#include <stdio.h> // Librería estándar para entrada/salida
#include <pthread.h> // Librería para manejo de hilos y sincronización
#include <unistd.h> // Librería para llamadas al sistema
#include <stdlib.h> // Librería para funciones básicas
#include <time.h> // Librería para manejo de tiempo
#include <sys/time.h> // Librería para medir el tiempo de ejecución

#define DATA_SIZE (1024*1024*64*3) // Tamaño total de la memoria asignada para las matrices

pthread_mutex_t MM_mutex; // Mutex para sincronización entre hilos
static double MEM_CHUNK[DATA_SIZE]; // Memoria estática para las matrices
double *mA, *mB, *mC; // Punteros a matrices

// Estructura de parámetros para pasar información a los hilos
struct parametros{
    int nH; // Número de hilos
    int idH; // ID del hilo
    int N; // Tamaño de la matriz
};

struct timeval start, stop; // Estructuras para medir el tiempo de ejecución

// Función para inicializar las matrices con valores predefinidos
void llenar_matriz(int SZ){ 
    srand48(time(NULL)); // Inicializa el generador de números aleatorios
    for(int i = 0; i < SZ*SZ; i++){
        mA[i] = 1.1*i; // Llenar matriz A
        mB[i] = 2.2*i; // Llenar matriz B
        mC[i] = 0; // Inicializar matriz C en cero
    }    
}

// Función para imprimir la matriz si el tamaño es pequeño
void print_matrix(int sz, double *matriz){
    if(sz < 12){ // Solo imprimir si el tamaño de la matriz es menor a 12x12
        for(int i = 0; i < sz*sz; i++){
            if(i%sz == 0) printf("\n"); // Salto de línea por fila
            printf(" %.3f ", matriz[i]); // Imprimir el valor con formato
        }    
        printf("\n>-------------------->\n"); // Separador entre matrices
    }
}

// Iniciar el cronómetro para medir tiempo
void inicial_tiempo(){
    gettimeofday(&start, NULL); // Guardar el tiempo de inicio
}

// Terminar el cronómetro y mostrar el tiempo transcurrido en microsegundos
void final_tiempo(){
    gettimeofday(&stop, NULL); // Guardar el tiempo final
    stop.tv_sec -= start.tv_sec; // Calcular la diferencia en segundos
    printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec)); // Imprimir tiempo
}

// Función de multiplicación de matrices que ejecutará cada hilo
void *mult_thread(void *variables){
    struct parametros *data = (struct parametros *)variables; // Obtener los parámetros

    int idH = data->idH; // ID del hilo
    int nH  = data->nH; // Número total de hilos
    int N   = data->N; // Tamaño de la matriz
    int ini = (N / nH) * idH; // Índice de inicio de filas que procesará este hilo
    int fin = (N / nH) * (idH + 1); // Índice de fin de filas que procesará este hilo

    for (int i = ini; i < fin; i++){ // Recorre las filas asignadas al hilo
        for (int j = 0; j < N; j++){ // Recorre cada columna de la matriz B
            double *pA, *pB, sumaTemp = 0.0; // Punteros temporales y suma temporal
            pA = mA + (i * N); // Posición de inicio de la fila en A
            pB = mB + (j * N); // Posición de inicio de la columna en B
            for (int k = 0; k < N; k++, pA++, pB++){ // Realiza la multiplicación y suma
                sumaTemp += (*pA * *pB); // Acumula el producto
            }
            mC[i * N + j] = sumaTemp; // Almacena el resultado en C
        }
    }

    pthread_mutex_lock(&MM_mutex); // Bloquear el mutex
    pthread_mutex_unlock(&MM_mutex); // Desbloquear el mutex
    pthread_exit(NULL); // Terminar el hilo
}

int main(int argc, char *argv[]){
    if (argc < 2){ // Verifica que se proporcionen argumentos
        printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n"); // Instrucción de uso
        return -1;    
    }
    int SZ = atoi(argv[1]); // Tamaño de la matriz
    int n_threads = atoi(argv[2]); // Número de hilos

    pthread_t p[n_threads]; // Arreglo de identificadores de hilos
    pthread_attr_t atrMM; // Atributos de los hilos

    // Asignación de memoria a las matrices A, B, y C en el bloque de memoria estática
    mA = MEM_CHUNK;
    mB = mA + SZ * SZ;
    mC = mB + SZ * SZ;

    llenar_matriz(SZ); // Llenar matrices con valores iniciales
    print_matrix(SZ, mA); // Imprimir matriz A
    print_matrix(SZ, mB); // Imprimir matriz B

    inicial_tiempo(); // Iniciar cronómetro para medir tiempo de ejecución
    pthread_mutex_init(&MM_mutex, NULL); // Inicializar el mutex
    pthread_attr_init(&atrMM); // Inicializar atributos de hilos
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE); // Hilos unibles

    // Creación de los hilos
    for (int j = 0; j < n_threads; j++){
        struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); // Reserva de memoria para parámetros
        datos->idH = j; // Asignación de ID de hilo
        datos->nH  = n_threads; // Número total de hilos
        datos->N   = SZ; // Tamaño de la matriz
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos); // Creación del hilo
    }

    // Esperar a que todos los hilos terminen
    for (int j = 0; j < n_threads; j++)
        pthread_join(p[j], NULL); // Unir cada hilo al hilo principal
    final_tiempo(); // Terminar cronómetro y mostrar tiempo de ejecución

    print_matrix(SZ, mC); // Imprimir matriz C

    pthread_attr_destroy(&atrMM); // Liberar los atributos de hilo
    pthread_mutex_destroy(&MM_mutex); // Destruir el mutex
    pthread_exit(NULL); // Terminar el programa principal
}