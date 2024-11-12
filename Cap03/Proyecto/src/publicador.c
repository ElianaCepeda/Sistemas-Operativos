/**************************************************************
		Pontificia Universidad Javeriana
	Autor: Daniel Castro, Maria Ruiz, Daniel Gonzalez, Eliana Cepeda
	Fecha: Noviembre 2024
	Materia: Sistemas Operativos
	Tema: Este proyecto implementa un sistema de distribución de noticias usando procesos en C 
		y comunicación mediante pipes nominales (FIFOs). El sistema permite que un publicador envíe 
		noticias a un sistema de comunicación, el cual las distribuye a los suscriptores interesados en temas específicos.
****************************************************************/
#include <stdio.h>              // Biblioteca estándar para funciones de entrada/salida
#include <stdlib.h>             // Biblioteca estándar para funciones generales de C
#include <unistd.h>             // Biblioteca POSIX para funciones de manipulación de archivos y procesos
#include <fcntl.h>              // Biblioteca para control de archivos (flags como O_WRONLY, O_NONBLOCK)
#include <string.h>             // Biblioteca para manejo de cadenas de caracteres
#include "utils.h"              // Incluye el archivo utils.h, que contiene funciones auxiliares como `is_valid_news_format`

#define BUFFER_SIZE 1024        // Define el tamaño del buffer para leer líneas del archivo

int main(int argc, char *argv[]) {
    char *pipePSC = NULL;       // Almacena el nombre del pipe a utilizar
    char *filename = NULL;      // Almacena el nombre del archivo de noticias
    int timeN = 1;              // Intervalo de tiempo entre envíos, por defecto 1 segundo

    // Procesa los argumentos de la línea de comandos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {   // Opción -p para especificar el pipe
            pipePSC = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) { // Opción -f para especificar el archivo
            filename = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) { // Opción -t para especificar el intervalo de tiempo
            timeN = atoi(argv[++i]);
        }
    }

    // Verifica que se hayan proporcionado `pipePSC` y `filename`
    if (!pipePSC || !filename) {
        fprintf(stderr, "Usage: publicador -p pipePSC -f file -t timeN\n");
        exit(EXIT_FAILURE);     // Sale si falta un argumento requerido
    }

    // Intenta abrir el archivo de noticias
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening news file");  // Muestra un mensaje de error si no se puede abrir el archivo
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];   // Define el buffer para almacenar cada línea del archivo de noticias
    while (fgets(buffer, sizeof(buffer), file)) {  // Lee cada línea del archivo
        if (is_valid_news_format(buffer)) {  // Verifica si la línea tiene el formato correcto usando una función de utils.h
            // Abre el pipe para escribir una noticia
            int fd = open(pipePSC, O_WRONLY | O_NONBLOCK);
            if (fd == -1) {     // Muestra un mensaje de error si no se puede abrir el pipe
                perror("Error opening pipePSC");
                fclose(file);   // Cierra el archivo antes de salir
                exit(EXIT_FAILURE);
            }
            write(fd, buffer, strlen(buffer));  // Escribe la noticia en el pipe
            close(fd);          // Cierra el pipe para permitir que `sistema` procese cada noticia

            printf("Noticia enviada: %s", buffer); // Imprime un mensaje confirmando el envío de la noticia
            sleep(timeN);       // Espera el tiempo especificado `timeN` antes de enviar la siguiente noticia
        } else {
            fprintf(stderr, "Invalid news format: %s", buffer);  // Muestra un mensaje si el formato de la noticia es incorrecto
        }
    }

    fclose(file);               // Cierra el archivo después de enviar todas las noticias
    return 0;                   // Termina el programa exitosamente
}
