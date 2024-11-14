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

Suscriptor suscriptores[MAX_SUSCRIPTORES]; // Array de suscriptores
Noticia noticias[MAX_NOTICIAS];            // Array de noticias
int num_suscriptores = 0;                  // Número de suscriptores actuales
int num_noticias = 0;                      // Número de noticias almacenadas
time_t last_news_time;                     // Marca de tiempo de la última noticia recibida

// Almacena una noticia en el array `noticias`
void almacenar_noticia(const char *noticia_text) {
    if (num_noticias >= MAX_NOTICIAS) {    // Verifica si el almacenamiento está lleno
        fprintf(stderr, "News storage is full, cannot store more news.\n");
        return;
    }
    noticias[num_noticias].tipo = noticia_text[0];  // El tipo de noticia es el primer carácter
    strncpy(noticias[num_noticias].contenido, noticia_text, BUFFER_SIZE);  // Almacena el contenido de la noticia
    num_noticias++;                       // Incrementa el contador de noticias
    last_news_time = time(NULL);          // Actualiza el tiempo de la última noticia
}

// Envía una noticia a un suscriptor si coincide con sus temas de interés
void enviar_noticia_a_suscriptor(const Suscriptor *suscriptor, const Noticia *noticia) {
    if (strchr(suscriptor->topics, noticia->tipo)) {  // Verifica si el tema coincide
        int fd = open(suscriptor->pipe_name, O_WRONLY | O_NONBLOCK); // Abre el pipe del suscriptor
        if (fd != -1) {           // Si el pipe se abre correctamente, envía la noticia
            write(fd, noticia->contenido, strlen(noticia->contenido));
            close(fd);            // Cierra el pipe después de enviar la noticia
        } else {
            perror("Error opening subscriber pipe");  // Mensaje de error si no se puede abrir el pipe
        }
    }
}

// Registra un nuevo suscriptor a partir de un mensaje recibido
void registrar_suscriptor(const char *message) {
    if (num_suscriptores >= MAX_SUSCRIPTORES) {  // Verifica si se alcanzó el máximo de suscriptores
        fprintf(stderr, "Max subscribers reached.\n");
        return;
    }
    char *delimiter = strchr(message, ':');  // Busca el delimitador ":" en el mensaje
    if (!delimiter) return;                 // Si no se encuentra, termina la función
    *delimiter = '\0';                      // Separa el nombre del pipe y los temas
    strncpy(suscriptores[num_suscriptores].pipe_name, message, BUFFER_SIZE); // Almacena el nombre del pipe

    // Normaliza los temas eliminando espacios
    char *topics = delimiter + 1;
    for (int i = 0, j = 0; topics[i] != '\0'; i++) {
        if (topics[i] != ' ') {              // Solo guarda caracteres distintos de espacios
            suscriptores[num_suscriptores].topics[j++] = topics[i];
        }
    }
    printf("Subscriber registered: Pipe=%s Topics=%s\n",
           suscriptores[num_suscriptores].pipe_name,
           suscriptores[num_suscriptores].topics);

    // Envía todas las noticias relevantes al nuevo suscriptor
    for (int i = 0; i < num_noticias; i++) {
        enviar_noticia_a_suscriptor(&suscriptores[num_suscriptores], &noticias[i]);
    }

    num_suscriptores++;                      // Incrementa el contador de suscriptores
}

// Envía un mensaje de final de transmisión a todos los suscriptores
void finalizar_transmision() {
    const char *end_message = "End of news broadcast.\n";
    printf("No new publishers detected. Broadcasting end of transmission.\n");
    for (int i = 0; i < num_suscriptores; i++) {
        int fd = open(suscriptores[i].pipe_name, O_WRONLY | O_NONBLOCK); // Abre el pipe del suscriptor
        if (fd != -1) {
            write(fd, end_message, strlen(end_message));  // Envía el mensaje de fin de transmisión
            close(fd);            // Cierra el pipe después de enviar
        } else {
            perror("Error opening subscriber pipe for final message");  // Error si no se puede abrir el pipe
        }
    }
}