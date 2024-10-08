
/*******************************************************************
Autor: Eliana Cepeda
Materia: Sistemas Operativos
Fecha: 1-Agosto-2024
Tema: Concatenación de strings
  - Uso de funciones para concaterar strings
********************************************************************/
#include <stdio.h>
#include <string.h>

char** function(char *p) {
  char q[strlen(p)+1];
  strcpy(q, p);
  printf("From q: the string is %s\n", q);
  return &q;
}

int main() {
  char **a = NULL;
  char **b = NULL;
  a = function("Hi, It's fun to learn");
  b = function("systems engineer");
  printf("From main: %s %s\n", *a, *b);
}


