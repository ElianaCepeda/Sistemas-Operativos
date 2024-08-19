#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int x;
  int suma=0;
  do{
    int d = scanf("%d", &x);
    suma+=x;
  } while(x!=0);
  printf("La suma es: %d \n", suma);
  return 0;
}