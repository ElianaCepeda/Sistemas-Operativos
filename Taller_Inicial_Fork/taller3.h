/************************************************************************************************
Fecha: 12/10/2024
Autores: Daniel Castro, María Paula Rodríguez, Eliana Cepeda, Daniel Gómez
Materia: Sistemas Operativos
Tema: Tercer Taller - fork()

Descripción:
- Este archivo contiene las definiciones de los prototipos de las funciones que se utilizarán
  para la lectura de arreglos desde archivos de texto y la suma de los elementos del arreglo.
  Es una parte fundamental de la modularización del código, permitiendo que estas funciones
  se declaren de forma global y sean accesibles desde otros archivos del proyecto.
************************************************************************************************/

#ifndef _TALLER3_H_
#define _TALLER3_H_

// Función que lee un arreglo de enteros desde un archivo de texto
// Parámetros:
//   archivo: nombre del archivo desde donde se leerá el arreglo
//   n: puntero al tamaño del arreglo
//   arreglo: doble puntero que almacenará el arreglo leído
void leer_arreglo(const char* archivo, int* n, int** arreglo);

// Función que realiza la suma de todos los elementos de un arreglo de enteros
// Parámetros:
//   arreglo: puntero al arreglo de enteros
//   n: tamaño del arreglo
// Retorna: el total de la suma de los elementos del arreglo
int suma(int* arreglo, int n);

#endif