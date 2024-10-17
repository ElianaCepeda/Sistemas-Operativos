/******************************************************************
Estudiante: Eliana Cepeda
Fecha: 29-08-2024
Materia: Sistemas Operativos
Tema: Laboratorio Compilación modular
  - Funcionamiento del codigo
  - Modularización del codigo
  - Creacion de fichero de Automatizacion de compilación
*******************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "Libreria_lab.h"


int main(){
        int i=0;
        
        vectorDinamico editor;
        vectorInicio(&editor);

        addVector(&editor, "Hola");
        addVector(&editor, "Profesional");
        addVector(&editor, "en");
        addVector(&editor, "Formación");
        addVector(&editor, "de");
        addVector(&editor, "Ingeniería");

        printf("\n");

        for (i = 0; i < totalVector(&editor); i++)
        printf("%s ", (char *) getVector(&editor, i));
        printf("\n");

        int j=5;
        while(j>=2){
                borrarVector(&editor, j);
                j--;
        }

        setVector(&editor, 1, "Buenos");
        addVector(&editor, "días");
        
        printf("\n");
        for (i = 0; i < totalVector(&editor); i++)
        printf("%s ", (char *) getVector(&editor, i));

        printf("\n");
        
        freeVector( &editor);

        return 0;
}