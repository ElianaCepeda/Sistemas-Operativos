/*******************************************************************
Autor: Eliana Cepeda
Materia: Sistemas Operativos
Fecha: 24-Octubre-2024
Tema: pthreads (hilos y procesos paralelos)

        ******Productor y Consumidor*****
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Variables compartidas
int counter = 0; // Contador compartido entre los hilos
int max = 4;     // Número máximo de hilos productor y consumidor

// Inicialización de los mutex para proteger las secciones críticas
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condp_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condc_mutex = PTHREAD_MUTEX_INITIALIZER;

// Inicialización de las variables de condición para la sincronización
pthread_cond_t condVarProd = PTHREAD_COND_INITIALIZER; // Variable para productor
pthread_cond_t condVarCons = PTHREAD_COND_INITIALIZER; // Variable para consumidor

// Función del hilo productor
void *productor() {
    while(1) {
        // Bloquea el mutex del productor para comprobar la condición
        pthread_mutex_lock(&condp_mutex);
        // Espera si el contador alcanza el límite máximo de 10
        while(counter >= 10) {
            pthread_cond_wait(&condVarProd, &condp_mutex); // Espera hasta que el consumidor lo despierte
        }
        pthread_mutex_unlock(&condp_mutex);

        // Bloquea el mutex del contador para actualizar el valor de forma segura
        pthread_mutex_lock(&counter_mutex);
        counter++; // Incrementa el contador
        pthread_cond_signal(&condVarCons); // Despierta al consumidor
        printf("Soy productor %d valor contador = %d\n", (int)pthread_self(), counter);
        pthread_mutex_unlock(&counter_mutex);
        pthread_mutex_unlock(&condp_mutex);

        // Si el contador llega a 5, espera un segundo antes de continuar
        if(counter == 5)
            sleep(1);
    }
}

// Función del hilo consumidor
void *consumidor() {
    while(1) {
        sleep(1); // Simula una pausa entre cada ciclo de consumo
        // Bloquea el mutex del consumidor para comprobar la condición
        pthread_mutex_lock(&condc_mutex);
        // Si el contador es menor o igual a 0, avisa al productor y espera
        while(counter <= 0) {
            pthread_cond_signal(&condVarProd); // Despierta al productor si está esperando
            pthread_cond_wait(&condVarCons, &condc_mutex); // Espera a que el productor incremente el contador
        }
        pthread_mutex_unlock(&condc_mutex);

        // Bloquea el mutex del contador para actualizar el valor de forma segura
        pthread_mutex_lock(&counter_mutex);
        if(counter > 0) {
            // Consume, es decir, decrementa el contador
            printf("Soy consumidor %d valor contador = %d\n", (int)pthread_self(), counter);
            counter--;
            pthread_cond_signal(&condVarProd); // Despierta al productor si estaba esperando
        }
        pthread_mutex_unlock(&counter_mutex);
        pthread_mutex_unlock(&condc_mutex);
    }
}

int main() {
    pthread_t proHilo[max], conHilo[max]; // Arreglo de hilos para productor y consumidor
    int i;

    // Crear 5 hilos productores y 5 hilos consumidores
    for(i = 0; i < 5; i++) {
        pthread_create(&proHilo[i], NULL, &productor, NULL);
        pthread_create(&conHilo[i], NULL, &consumidor, NULL);
    }

    // Espera a que los hilos terminen su ejecución
    for(i = 0; i < 5; i++) {
        pthread_join(proHilo[i], NULL);
        pthread_join(conHilo[i], NULL);
    }

    return 0;
}