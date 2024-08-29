/******************************************************************
Estudiante: Eliana Cepeda
Fecha: 29-08-2024
Materia: Sistemas Operativos
Tema: Laboratorio Compilación modular
  - Funcionamiento del codigo
  - Modularización del codigo
  - Creacion de fichero de Automatizacion de compilación

*******************************************************************
        INTERFAZ DE LIBRERIA DE FUNCIONES: MENÚ DE FUNCIONES
*******************************************************************/
#ifndef _LIBRERIA_LAB_H_
#define _LIBRERIA_LAB_H_

#define CAPACIDAD_INICIAL 4

/*Creación Estructura de Datos para el Vector*/
typedef struct vectorDinamico{
        int capacidad;
        int totalElementos;
        void **elementos;
} vectorDinamico;

// Inicialización de los valores del vector, casting y reserva de memoria
void vectorInicio(vectorDinamico *V);

// Retorna el total de elemtos
int totalVector(vectorDinamico *V);

//Redimensiona el vector
static void resizeVector(vectorDinamico *V, int capacidad);

//Agrega elementos al vector
void addVector(vectorDinamico *V, void *elemento);

// Liberación de memoria dinamica
void freeVector(vectorDinamico *V);

// Retorna el elemento en determinada posición
void *getVector(vectorDinamico *V, int indice);

// Modifica el valor de una posición especifica del vector
void setVector(vectorDinamico *V, int indice, void *elemento);

// Borra elemento según inidice y redimenciona el vector        
void borrarVector(vectorDinamico *V, int indice);

#endif