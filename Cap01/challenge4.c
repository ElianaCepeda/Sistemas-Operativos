
/*******************************************************************
Autor: Eliana Cepeda
Materia: Sistemas Operativos
Fecha: 1-Agosto-2024
Tema: Asignacion de memoria
  - uso de calloc
********************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main() {
  int n, i, *ptr, sum = 0;
  printf("Enter number of elements: ");
  int b =scanf("%d", &n);
  ptr = (int*) calloc(n, sizeof(int));
  if(ptr == NULL) {
    printf("Error! memory not allocated.");
    exit(0);
  }
  printf("Enter elements: ");
  for(i = 0; i < n; ++i) {
    int a =scanf("%d", ptr + i);
    sum += *(ptr + i);
  }
  printf("Sum = %d", sum);
  free(ptr);
  return 0;
return 0;
}

