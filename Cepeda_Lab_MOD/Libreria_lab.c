/******************************************************************
Estudiante: Eliana Cepeda
Fecha: 29-08-2024
Materia: Sistemas Operativos
Tema: Laboratorio Compilación modular
  - Funcionamiento del codigo
  - Modularización del codigo
  - Creacion de fichero de Automatizacion de compilación

*******************************************************************
        LIBRERIA DE FUNCIONES: IMPLEMETEACIÓN DE FUNCIONES
*******************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "Libreria_lab.h"


// Inicialización de los valores del vector, casting y reserva de memoria
void vectorInicio(vectorDinamico *V){
        V->capacidad = CAPACIDAD_INICIAL;
        V->totalElementos = 0;
        V->elementos = malloc(sizeof(void *) * V->capacidad); 
}

// Retorna el total de elemtos
int totalVector(vectorDinamico *V){
        return V->totalElementos;
}

//Redimensiona el vector
static void resizeVector(vectorDinamico *V, int capacidad){
        printf("Redimensión: %d a %d \n", V->capacidad, capacidad);

        void **elementos = realloc(V->elementos, sizeof(void *) * capacidad);
        if(elementos){
                V->elementos = elementos;
                V->capacidad = capacidad;
        }
}

//Agrega elementos al vector
void addVector(vectorDinamico *V, void *elemento){
        if(V->capacidad == V->totalElementos)
                resizeVector(V, V->capacidad*2);
        V->elementos[V->totalElementos++] = elemento;
}

// Liberación de memoria dinamica
void freeVector(vectorDinamico *V){
        free(V->elementos);
}

// Retorna el elemento en determinada posición
void *getVector(vectorDinamico *V, int indice){
        if(indice >= 0 && indice < V->totalElementos)
                return V->elementos[indice];
        return NULL;
}

// Modifica el valor de una posición especifica del vector
void setVector(vectorDinamico *V, int indice, void *elemento){
        if(indice >= 0 && indice < V->totalElementos)
                V->elementos[indice]=elemento;
}

// Borra elemento según inidice y redimenciona el vector        
void borrarVector(vectorDinamico *V, int indice){
        if(indice < 0 || indice >= V->totalElementos)
                return; 

        V->elementos[indice] = NULL;

        for(int i=indice; i<V->totalElementos-1; i++){
                V->elementos[i] = V->elementos[i+1];
                V->elementos[i+1] = NULL; 
        }
        V->totalElementos--;
        if(V->totalElementos>0 && V->totalElementos == V->capacidad/4)
                resizeVector(V, V->capacidad/2);
}

