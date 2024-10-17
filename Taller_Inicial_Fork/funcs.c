/************************************************************************************************
Fecha: 12/10/2024
Autores: Daniel Castro, María Paula Rodríguez, Eliana Cepeda, Daniel Gómez
Materia: Sistemas Operativos
Tema: Tercer Taller - fork()

Descripción:
- Este archivo implementa dos funciones fundamentales para el programa principal:
  1. La función `leer_arreglo()`, que lee un arreglo de enteros desde un archivo de texto.
  2. La función `suma()`, que realiza la suma de los elementos de un arreglo.
  Ambas funciones trabajan en conjunto con el programa principal, permitiendo la modularización
  y facilitando la manipulación de datos de los arreglos leídos.
************************************************************************************************/

#include <stdio.h>  // Biblioteca estándar de entrada/salida
#include <stdlib.h> // Biblioteca para manejo de memoria dinámica y control de procesos
#include "taller3.h"  // Inclusión del archivo de cabecera con las definiciones de las funciones

// Función que lee un arreglo de enteros desde un archivo de texto
void leer_arreglo(const char* archivo, int* n, int** arreglo) {

    // Apertura del archivo en modo lectura
    FILE* f = fopen(archivo, "r");
    
    // Verificación de errores en la apertura del archivo
    if (!f) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);  // Termina si no puede abrir el archivo
    }

    // Asignación de memoria dinámica para el arreglo
    *arreglo = (int*)malloc(*n * sizeof(int));
    
    // Verificación de la correcta asignación de memoria
    if (!*arreglo) {
        perror("Error al asignar memoria");
        fclose(f);  // Cierra el archivo antes de salir
        exit(EXIT_FAILURE);  // Termina si no se puede asignar la memoria
    }

    // Ciclo para leer los enteros desde el archivo y guardarlos en el arreglo
    for (int i = 0; i < *n; i++) {
        if (fscanf(f, "%d", &((*arreglo)[i])) != 1) {
            perror("Error al leer el archivo");
            free(*arreglo);  // Libera la memoria asignada si ocurre un error
            fclose(f);  // Cierra el archivo antes de salir
            exit(EXIT_FAILURE);  // Termina si hay un error en la lectura
        }
    }

    // Cierre del archivo después de la lectura
    fclose(f);
}

// Función que suma los elementos de un arreglo de enteros
int suma(int* arreglo, int n) {
    int total = 0;  // Inicializa la variable para almacenar el total

    // Ciclo para sumar cada elemento del arreglo
    for (int i = 0; i < n; i++) {
        total += arreglo[i];  // Acumula la suma de los elementos
    }

    return total;  // Retorna la suma total
}