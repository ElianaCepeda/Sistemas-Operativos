/************************************************************************************************
Fecha: 10/11/2024
Autor: Daniel Castro, María Paula Rodríguez, Eliana Cepeda, Daniel Gomez
Materia: Sistemas Operativos
Tema: Suscriptor de Noticias a través de Tuberías
Descripción:
    Este módulo contiene la función que valida el formato de una noticia recibida en el sistema.
    El formato válido de una noticia debe cumplir con los siguientes requisitos:
    1. Comenzar con una letra mayúscula.
    2. Tener una longitud mayor a 3 caracteres.
    3. El segundo carácter debe ser un colon ":".
    4. El penúltimo carácter debe ser un punto ".".
    Esta función es utilizada para asegurar que las noticias recibidas cumplen con el formato esperado
    antes de ser procesadas o distribuidas a los suscriptores.
************************************************************************************************
            VALIDACIÓN
************************************************************************************************/
#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <ctype.h>
#define MAX_SUSCRIPTORES 100

int is_valid_news_format(const char *news) { 
    // Verifica que el puntero de la noticia no sea nulo y que cumpla con las condiciones de formato especificadas.
    return (news && isupper(news[0]) && strlen(news) > 3 && news[1] == ':' && news[strlen(news) - 2] == '.'); // Retorna 1 si el formato es válido
}

#endif
