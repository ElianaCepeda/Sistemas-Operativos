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
#ifndef SCBIBLIOTECA_H
#define SCBIBLIOTECA_H

#include <stdio.h>              // Biblioteca estándar para funciones de entrada/salida
#include <stdlib.h>             // Biblioteca estándar para funciones generales de C
#include <string.h>             // Biblioteca para manejo de cadenas de caracteres
#include <unistd.h>             // Biblioteca POSIX para manejo de archivos y procesos
#include <fcntl.h>              // Biblioteca para control de archivos (flags como O_WRONLY, O_NONBLOCK)
#include <sys/stat.h>           // Biblioteca para manejo de archivos (permisos y tipos de archivos)
#include <errno.h>              // Biblioteca para manejo de errores
#include <sys/select.h>         // Biblioteca para monitorear múltiples descriptores de archivos
#include <time.h>               // Biblioteca para funciones y estructuras de tiempo

#define BUFFER_SIZE 1024        // Tamaño del buffer para lectura y escritura
#define MAX_SUSCRIPTORES 100    // Número máximo de suscriptores
#define MAX_NOTICIAS 100        // Número máximo de noticias almacenadas

// Estructura para almacenar una noticia
typedef struct {
    char tipo;                    // Tipo de noticia (e.g., 'A', 'E')
    char contenido[BUFFER_SIZE];  // Descripción de la noticia
} Noticia;

// Estructura para almacenar un suscriptor
typedef struct {
    char topics[BUFFER_SIZE];     // Temas de interés del suscriptor (e.g., "A E")
    char pipe_name[BUFFER_SIZE];  // Nombre del pipe único del suscriptor
} Suscriptor;

extern Suscriptor suscriptores[MAX_SUSCRIPTORES]; // Array de suscriptores
extern Noticia noticias[MAX_NOTICIAS];            // Array de noticias
extern int num_suscriptores;                  // Número de suscriptores actuales
extern int num_noticias;                      // Número de noticias almacenadas
extern time_t last_news_time;                     // Marca de tiempo de la última noticia recibida

// Almacena una noticia en el array `noticias`
void almacenar_noticia(const char *noticia_text);

// Envía una noticia a un suscriptor si coincide con sus temas de interés
void enviar_noticia_a_suscriptor(const Suscriptor *suscriptor, const Noticia *noticia);

// Registra un nuevo suscriptor a partir de un mensaje recibido
void registrar_suscriptor(const char *message);

// Envía un mensaje de final de transmisión a todos los suscriptores
void finalizar_transmision();

#endif