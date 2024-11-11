/************************************************************************************************
Fecha: 10/11/2024
Autor: Daniel Castro, María Paula Rodríguez, Eliana Cepeda, Daniel Gomez
Materia: Sistemas Operativos
Tema: Suscriptor de Noticias a través de Tuberías
Descripción:
    Este programa permite a un suscriptor recibir noticias de una tubería compartida. El suscriptor 
    se suscribe a ciertos temas y recibe noticias relacionadas a esos temas. El programa crea una 
    tubería única para cada suscriptor, envía la solicitud de suscripción a la tubería compartida 
    y luego lee las noticias en esa tubería hasta que recibe el mensaje de fin de la transmisión.
************************************************************************************************
            SUSCRIPTOR
************************************************************************************************/
/* Importación de librerías */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    char *pipeSSC = NULL;  // Nombre de la tubería compartida para enviar temas de suscripción

    // Procesa los argumentos de línea de comandos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {  // -s indica la tubería SSC
            pipeSSC = argv[++i];
        }
    }
    
    // Verifica que se haya especificado la tubería de suscripción
    if (!pipeSSC) {
        fprintf(stderr, "Usage: suscriptor -s pipeSSC\n");
        exit(EXIT_FAILURE);
    }

    // Genera un nombre único para la tubería del suscriptor en /tmp
    char pipeUnique[BUFFER_SIZE];
    snprintf(pipeUnique, sizeof(pipeUnique), "/tmp/pipeS_%d", getpid());
    if (mkfifo(pipeUnique, 0666) == -1 && errno != EEXIST) {  // Crea la tubería única
        perror("Error creating unique pipe in /tmp");
        exit(EXIT_FAILURE);
    }

    // Abre la tubería compartida para enviar la solicitud de suscripción
    int fd_shared = open(pipeSSC, O_WRONLY);
    if (fd_shared == -1) {
        unlink(pipeUnique);
        perror("Error opening shared pipe for topics");
        exit(EXIT_FAILURE);
    }

    // Solicita los temas de interés al usuario
    char topics[BUFFER_SIZE];
    printf("Enter topics of interest (e.g., A E S): ");
    fgets(topics, sizeof(topics), stdin);
    topics[strcspn(topics, "\n")] = '\0';  // Elimina el salto de línea

    // Envía la información de suscripción en el formato "pipeUnique:topics"
    dprintf(fd_shared, "%s:%s\n", pipeUnique, topics);
    close(fd_shared);

    // Abre la tubería única para leer las noticias en modo bloqueante
    int fd = open(pipeUnique, O_RDONLY);
    if (fd == -1) {
        unlink(pipeUnique);
        perror("Error opening unique pipe for reading news");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for news on selected topics...\n");
    char buffer[BUFFER_SIZE];

    // Lee las noticias hasta recibir el mensaje "End of news broadcast"
    while (1) {
        int bytesRead = read(fd, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';  // Asegura que la cadena esté bien terminada

            // Verifica si se ha recibido el mensaje de fin de transmisión
            if (strcmp(buffer, "End of news broadcast.\n") == 0) {
                printf("%s", buffer);  // Muestra el mensaje de fin
                break;  // Sale después de recibir el mensaje de fin
            }

            printf("Received news: %s\n", buffer);  // Muestra las noticias recibidas
        } else if (bytesRead < 0) {
            perror("Error reading from unique pipe");
            break;  // Salir en caso de error al leer
        }
    }

    close(fd);
    unlink(pipeUnique);  // Elimina la tubería única después de terminar
    return 0;
}