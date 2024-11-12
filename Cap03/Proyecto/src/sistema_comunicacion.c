/**************************************************************
		Pontificia Universidad Javeriana
	Autor: Daniel Castro, Maria Ruiz, Daniel Gonzalez, Eliana Cepeda
	Fecha: Noviembre 2024
	Materia: Sistemas Operativos
	Tema: Este proyecto implementa un sistema de distribución de noticias usando procesos en C 
		y comunicación mediante pipes nominales (FIFOs). El sistema permite que un publicador envíe 
		noticias a un sistema de comunicación, el cual las distribuye a los suscriptores interesados en temas específicos.
****************************************************************/
#include <stdio.h>              // Incluye funciones estándar de entrada/salida
#include <stdlib.h>             // Incluye funciones estándar de C
#include <string.h>             // Incluye funciones para manejo de cadenas
#include <fcntl.h>              // Incluye constantes y funciones para control de archivos
#include <unistd.h>             // Incluye funciones POSIX, como `read` y `write`
#include <sys/stat.h>           // Incluye constantes para operaciones de estado de archivos
#include <errno.h>              // Incluye el manejo de códigos de error

#define BUFFER_SIZE 1024        // Tamaño máximo del buffer para lectura/escritura

int main(int argc, char *argv[]) {
    char *pipeSSC = NULL;       // Puntero para almacenar el nombre del pipe compartido
    for (int i = 1; i < argc; i++) {  // Procesa los argumentos de línea de comandos
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            pipeSSC = argv[++i];  // Asigna el siguiente argumento como `pipeSSC` si se encuentra `-s`
        }
    }
    if (!pipeSSC) {              // Verifica que se haya proporcionado `pipeSSC`
        fprintf(stderr, "Usage: suscriptor -s pipeSSC\n");
        exit(EXIT_FAILURE);       // Termina el programa si falta el argumento
    }

    // Genera una ruta única en /tmp para el pipe del suscriptor
    char pipeUnique[BUFFER_SIZE];
    snprintf(pipeUnique, sizeof(pipeUnique), "/tmp/pipeS_%d", getpid()); // Usa el PID para crear un nombre único
    if (mkfifo(pipeUnique, 0666) == -1 && errno != EEXIST) { // Crea el pipe y verifica errores
        perror("Error creating unique pipe in /tmp"); // Muestra mensaje de error si no se pudo crear el pipe
        exit(EXIT_FAILURE);
    }

    // Abre el pipe compartido para enviar temas de suscripción y el nombre del pipe único
    int fd_shared = open(pipeSSC, O_WRONLY);  // Abre `pipeSSC` en modo escritura
    if (fd_shared == -1) {        // Verifica si `pipeSSC` se abrió correctamente
        unlink(pipeUnique);       // Elimina `pipeUnique` si falla la apertura de `pipeSSC`
        perror("Error opening shared pipe for topics");
        exit(EXIT_FAILURE);
    }

    // Solicita al usuario los temas de interés
    char topics[BUFFER_SIZE];
    printf("Enter topics of interest (e.g., A E S): ");
    fgets(topics, sizeof(topics), stdin);      // Lee los temas de la entrada estándar
    topics[strcspn(topics, "\n")] = '\0';      // Elimina el carácter de nueva línea

    // Envía los datos de suscripción en formato "pipeUnique:topics"
    dprintf(fd_shared, "%s:%s\n", pipeUnique, topics);  // Escribe en `pipeSSC`
    close(fd_shared);           // Cierra el pipe compartido

    // Abre el pipe único en modo lectura bloqueante para recibir noticias
    int fd = open(pipeUnique, O_RDONLY);       // Abre `pipeUnique` en modo lectura
    if (fd == -1) {           // Verifica si `pipeUnique` se abrió correctamente
        unlink(pipeUnique);    // Elimina `pipeUnique` si falla la apertura
        perror("Error opening unique pipe for reading news");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for news on selected topics...\n");
    char buffer[BUFFER_SIZE];  // Buffer para almacenar las noticias recibidas

    // Lee noticias hasta recibir el mensaje de "End of news broadcast"
    while (1) {
        int bytesRead = read(fd, buffer, sizeof(buffer) - 1);  // Lee datos de `pipeUnique`
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';     // Agrega terminador nulo a la cadena leída

            // Verifica si el mensaje es el fin de la transmisión
            if (strcmp(buffer, "End of news broadcast.\n") == 0) {
                printf("%s", buffer);     // Muestra el mensaje de fin de transmisión
                break;                    // Termina el bucle al recibir el mensaje de fin
            }

            printf("Received news: %s\n", buffer); // Muestra las noticias recibidas
        } else if (bytesRead < 0){        // Si ocurre un error en la lectura
            perror("Error reading from unique pipe"); // Muestra mensaje de error
            break;
        }
    }

    close(fd);                 // Cierra `pipeUnique`
    unlink(pipeUnique);        // Elimina `pipeUnique` después de finalizar
    return 0;                  // Termina el programa exitosamente
}
