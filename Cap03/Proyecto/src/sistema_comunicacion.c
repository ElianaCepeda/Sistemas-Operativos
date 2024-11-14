/**************************************************************
		Pontificia Universidad Javeriana
	Autor: Daniel Castro, Maria Ruiz, Daniel Gonzalez, Eliana Cepeda
	Fecha: Noviembre 2024
	Materia: Sistemas Operativos
	Tema: Proyecto Sistema de Comunicacion
	Objetivo: Este proyecto implementa un sistema de distribución de noticias usando procesos en C 
    y comunicación mediante pipes nominales (FIFOs). El sistema permite que un `publicador` envíe 
    noticias a un `sistema de comunicación`, el cual las distribuye a los `suscriptores` interesados en temas específicos.
****************************************************************/

#include <stdio.h>              // Biblioteca estándar para funciones de entrada/salida
#include <stdlib.h>             // Biblioteca estándar para funciones generales de C
#include <string.h>             // Biblioteca para manejo de cadenas de caracteres
#include <unistd.h>             // Biblioteca POSIX para manejo de archivos y procesos
#include <fcntl.h>              // Biblioteca para control de archivos (flags como O_WRONLY, O_NONBLOCK)
#include <sys/stat.h>           // Biblioteca para manejo de archivos (permisos y tipos de archivos)
#include <errno.h>              // Biblioteca para manejo de errores
#include <sys/select.h>         // Biblioteca para monitorear múltiples descriptores de archivos
#include <time.h>               // Biblioteca para funciones y estructuras de tiempo
#include "SCBiblioteca.h"

// Función principal del sistema de distribución de noticias
int main(int argc, char *argv[]) {
    char *pipePSC = NULL, *pipeSSC = NULL;  // Variables para almacenar los nombres de los pipes
    int timeF = 0;                          // Tiempo de inactividad para finalizar la transmisión

    // Procesa los argumentos de la línea de comandos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            pipePSC = argv[++i];
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            pipeSSC = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            timeF = atoi(argv[++i]);
        }
    }
    if (!pipePSC || !pipeSSC || timeF <= 0) { // Verifica que los parámetros necesarios estén presentes
        fprintf(stderr, "Usage: sistema -p pipePSC -s pipeSSC -t timeF\n");
        exit(EXIT_FAILURE);
    }

    mkfifo(pipePSC, 0666);  // Crea el pipe de publicador
    mkfifo(pipeSSC, 0666);  // Crea el pipe de suscriptor

    int fdPSC = open(pipePSC, O_RDONLY | O_NONBLOCK);  // Abre `pipePSC` en modo no bloqueante
    int fdSSC = open(pipeSSC, O_RDONLY | O_NONBLOCK);  // Abre `pipeSSC` en modo no bloqueante

    if (fdPSC == -1 || fdSSC == -1) {  // Verifica si los pipes se abrieron correctamente
        perror("Error opening pipes");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];          // Buffer para leer datos de los pipes
    fd_set read_fds;                   // Conjunto de descriptores para `select`
    int max_fd = (fdPSC > fdSSC ? fdPSC : fdSSC) + 1; // Calcula el descriptor máximo para `select`

    last_news_time = time(NULL);       // Inicializa el tiempo de la última noticia
    printf(">>> SYSTEM STARTED <<<\n");

    while (1) {
        FD_ZERO(&read_fds);            // Inicializa el conjunto de descriptores de lectura
        FD_SET(fdPSC, &read_fds);      // Agrega `fdPSC` al conjunto
        FD_SET(fdSSC, &read_fds);      // Agrega `fdSSC` al conjunto

        struct timeval timeout;
        timeout.tv_sec = 1;            // Tiempo de espera de 1 segundo
        timeout.tv_usec = 0;

        if (select(max_fd, &read_fds, NULL, NULL, &timeout) > 0) { // Monitorea los descriptores de archivo
            if (FD_ISSET(fdPSC, &read_fds)) {  // Verifica si hay datos en `fdPSC`
                int bytes_read = read(fdPSC, buffer, sizeof(buffer) - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';   // Agrega terminador nulo a la cadena leída
                    printf("News received from publisher: %s\n", buffer);
                    last_news_time = time(NULL); // Actualiza el tiempo de la última noticia
                    almacenar_noticia(buffer);   // Almacena la noticia

                    // Distribuye la noticia a todos los suscriptores
                    for (int i = 0; i < num_suscriptores; i++) {
                        enviar_noticia_a_suscriptor(&suscriptores[i], &noticias[num_noticias - 1]);
                    }
                }
            }
            if (FD_ISSET(fdSSC, &read_fds)) {  // Verifica si hay datos en `fdSSC`
                int bytes_read = read(fdSSC, buffer, sizeof(buffer) - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    registrar_suscriptor(buffer);  // Registra al suscriptor y envía noticias relevantes
                }
            }
        }

        // Verifica inactividad en `pipePSC` (pipe de publicador)
        if (difftime(time(NULL), last_news_time) >= timeF) {
            finalizar_transmision();   // Finaliza la transmisión si hay inactividad
            break;
        }
    }

    close(fdPSC);                      // Cierra `fdPSC`
    close(fdSSC);                      // Cierra `fdSSC`
    unlink(pipePSC);                   // Elimina `pipePSC`
    unlink(pipeSSC);                   // Elimina `pipeSSC`
    return 0;
}
