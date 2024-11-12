/**************************************************************
		Pontificia Universidad Javeriana
	Autor: Daniel Castro, Maria Ruiz, Daniel Gonzalez, Eliana Cepeda
	Fecha: Noviembre 2024
	Materia: Sistemas Operativos
	Tema: Este proyecto implementa un sistema de distribución de noticias usando procesos en C 
		y comunicación mediante pipes nominales (FIFOs). El sistema permite que un publicador envíe 
		noticias a un sistema de comunicación, el cual las distribuye a los suscriptores interesados en temas específicos.
****************************************************************/
#ifndef UTILS_H
#define UTILS_H

#include <string.h> // Incluye funciones para manipulación de cadenas

#define MAX_SUSCRIPTORES 100

// Función que valida el formato de las noticias.
int is_valid_news_format(const char *news) {
    // Verifica que la noticia no sea NULL, tenga longitud suficiente,
    // comience con uno de los caracteres válidos y el segundo carácter sea ':'
    return (news && strlen(news) > 3 && 
            (news[0] == 'A' || news[0] == 'E' || news[0] == 'C' || news[0] == 'P' || news[0] == 'S') &&
            news[1] == ':');
}

#endif
