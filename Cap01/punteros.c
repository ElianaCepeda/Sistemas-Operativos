#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  
  int variable =123;
  int *ptr = &variable;
  printf("valor de variable: %d\n", variable);
  printf("direccion de variable: %p\n", &variable);
  printf("Direccion a la que apunt el puntero %p\n", (void*)ptr);
  printf("valor de la variable %i \n", *ptr);
  return 0;
}