/************************************************************************************************
Fecha: 10/11/2024
Autor: Daniel Castro, María Paula Rodríguez, Eliana Cepeda, Daniel Gomez
Materia: Sistemas Operativos
Tema: Publicador de Noticias a través de Tuberías
Descripción:
    Este programa lee noticias de un archivo especificado y las envía a una tubería mediante un 
    intervalo de tiempo determinado. El programa también valida el formato de cada noticia antes 
    de enviarla.
************************************************************************************************
            PUBLICADOR
************************************************************************************************/
/* Importación de librerías */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "utils.h"

#define BUFFER_SIZE 1024  // Tamaño del buffer para leer líneas de noticias

int main(int argc, char *argv[]) {
    char *pipePSC = NULL;   // Nombre de la tubería para enviar noticias
    char *filename = NULL;   // Nombre del archivo de noticias
    int timeN = 1;           // Intervalo de tiempo entre envíos de noticias (en segundos)

    // Procesa los argumentos de línea de comandos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {      // -p indica el nombre de la tubería
            pipePSC = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) { // -f indica el archivo de noticias
            filename = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) { // -t indica el intervalo de tiempo
            timeN = atoi(argv[++i]);
        }
    }

    // Verifica que se hayan especificado la tubería y el archivo de noticias
    if (!pipePSC || !filename) {
        fprintf(stderr, "Usage: publicador -p pipePSC -f file -t timeN\n");
        exit(EXIT_FAILURE);
    }

    // Abre el archivo de noticias en modo de lectura
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening news file");  // Error si el archivo no se puede abrir
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];  // Buffer para almacenar cada noticia leída
    while (fgets(buffer, sizeof(buffer), file)) {  // Lee cada línea del archivo
        if (is_valid_news_format(buffer)) {  // Verifica que el formato de la noticia sea válido
            // Abre la tubería, escribe una noticia y luego la cierra
            int fd = open(pipePSC, O_WRONLY | O_NONBLOCK);  // Abre la tubería en modo escritura no bloqueante
            if (fd == -1) {
                perror("Error opening pipePSC");  // Error si la tubería no se puede abrir
                fclose(file);
                exit(EXIT_FAILURE);
            }
            write(fd, buffer, strlen(buffer));  // Envía la noticia por la tubería
            close(fd);  // Cierra la tubería para que el sistema procese la noticia

            printf("Noticia enviada: %s", buffer);  // Imprime la noticia enviada
            sleep(timeN);  // Espera el tiempo especificado antes de enviar la siguiente noticia
        } else {
            fprintf(stderr, "Invalid news format: %s", buffer);  // Informa de formato inválido
        }
    }

    fclose(file);  // Cierra el archivo de noticias
    return 0;
}