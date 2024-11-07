/*******************************************************************
 * Fecha: 7 de noviembre del 2024
 * Autor: Eliana Cepeda
 * Materia: Sistemas Operativos
* objetivo: Desarrollo de programa que usa hilos de forma paralela pero no acceden al mismo recurso de forma simultanea

****************************************************
* Descripción del Código
* 
* 1. Se incluyen las bibliotecas necesarias para la creación y gestión de hilos (pthread.h) y funciones de E/S (stdio.h).
* 2. Se declara un mutex global llamado BLOQUEO, que se utiliza para asegurar la exclusión mutua y evitar condiciones de carrera al acceder a secciones críticas del código.
* 3. La función "funcion" es la rutina que ejecutan los hilos. Realiza las siguientes tareas:
*    - Bloquea la sección crítica utilizando el mutex.
*    - Imprime un mensaje indicando el inicio de la ejecución del hilo.
*    - Realiza un ciclo de conteo simulado (300 iteraciones).
*    - Imprime un mensaje al finalizar la ejecución del hilo.
*    - Desbloquea el mutex, permitiendo que otros hilos puedan acceder a la sección crítica.
* 4. La función main se encarga de:
*    - Declarar e inicializar los arreglos para los identificadores de los hilos.
*    - Inicializar el mutex. Si la inicialización falla, se imprime un mensaje de error y se termina el programa.
*    - Crear cinco hilos en un bucle, pasando un identificador a cada hilo.
*    - Esperar a que todos los hilos terminen utilizando pthread_join después del bucle de creación, para maximizar el paralelismo.
* 5. Finalmente, el mutex se destruye para liberar los recursos asignados.
* 
* Notas:
* - El uso correcto de pthread_join después del bucle permite que los hilos se ejecuten en paralelo.
* - Esta estructura asegura que la sección crítica esté protegida y evita posibles condiciones de carrera.
****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

// Declaración de un mutex global para el control de acceso a secciones críticas
pthread_mutex_t BLOQUEO;

// Definición de la función que ejecutan los hilos
void *funcion(void *job) {
    // Bloqueo de la sección crítica
    pthread_mutex_lock(&BLOQUEO);

    // Obtención del identificador del hilo
    size_t IDh = *(size_t *)job;
    int cont = 0;

    // Impresión de inicio del hilo
    printf("\nHilo %zu ha iniciado\n", IDh);

    // Ciclo de conteo simulado
    for (int i = 0; i < 300; i++) {
        cont++;
    }

    // Impresión de finalización del hilo
    printf("\nHilo %zu ha finalizado\n", IDh);

    // Desbloqueo de la sección crítica
    pthread_mutex_unlock(&BLOQUEO);
}

int main(int argc, char **argv) {
    size_t jobs[5];            // Arreglo para almacenar identificadores de los hilos
    pthread_t hilos[5];        // Arreglo de identificadores de los hilos de tipo pthread_t
    int error;                 // Variable para capturar errores en la creación de hilos
    int i = 0;

    // Inicialización del mutex
    if (pthread_mutex_init(&BLOQUEO, NULL) != 0) {
        printf("\nFallo de iniciacion de MUTEX\n");
        return 1;
    }

    // Creación de los hilos
    while (i < 5) {
        jobs[i] = i;  // Asignación del identificador del hilo
        error = pthread_create(&hilos[i], NULL, &funcion, jobs + i);  // Creación del hilo
        if (error != 0) {
            printf("\nError en la creacion del hilo\n");
        }
        i++;
    }

    // Espera a que todos los hilos hayan terminado
    for (int i = 0; i < 5; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Destrucción del mutex
    pthread_mutex_destroy(&BLOQUEO);
}