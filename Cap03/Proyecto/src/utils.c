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
#include <string.h> // Incluye funciones para manipulación de cadenas
#include "utils.h"
#include <ctype.h>  // for isspace
#include <stdio.h>

#define BUFFER_SIZE 1024

// Function to trim whitespace from both ends of a string (modifies the string directly)
void trim_whitespace(char *str) {
    char *end;

    // Trim leading spaces
    while (isspace((unsigned char)*str)) str++;

    // If all spaces, set empty string
    if (*str == 0) {
        str[0] = '\0';
        return;
    }

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Set new null terminator
    *(end + 1) = '\0';
}

// Function to validate the news format directly on `buffer`
int is_valid_news_format(char *news) {
    // Trim whitespace from both ends directly on the input
    trim_whitespace(news);

    // Check format: sufficient length, starts with valid character, second character is ':', ends with '.'
    size_t len = strlen(news);
    int is_valid = (len > 3 && len < 80 && 
                    (news[0] == 'A' || news[0] == 'E' || news[0] == 'C' || 
                     news[0] == 'P' || news[0] == 'S') &&
                    news[1] == ':' &&
                    news[len - 1] == '.');
    
    // Debug message if the news format is invalid
    if (!is_valid) {
        fprintf(stderr, "Invalid news format (missing period or other error): %s\n", news);
    }
    
    return is_valid;
}
