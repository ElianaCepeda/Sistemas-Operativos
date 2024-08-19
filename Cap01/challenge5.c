/*******************************************************************
Autor: Eliana Cepeda
Materia: Sistemas Operativos
Fecha: 1-Agosto-2024
Tema: Asignación de memoria dinamica
	Uso funcion realoc para redimensionar la memoria asignada
********************************************************************/
#include <stdio.h>
#include <stdlib.h>

int main() {
int *ptr, i , n1, n2;
printf("Enter size: ");
int a = scanf("%d", &n1);
ptr = (int*) malloc(n1 * sizeof(int));
printf("Addresses of previously allocated memory: ");
for(i = 0; i < n1; ++i)
  printf("\n\np = %p\n", ptr+i);
printf("\n Enter the new size: ");
int b = scanf("%d", &n2);
// rellocating the memory
ptr = realloc(ptr, n2 * sizeof(int));
printf("Addresses of newly allocated memory: ");
for(i = 0; i < n2; ++i)
  printf("\n\np = %p\n", ptr+i);
  
free(ptr);
return 0;
}  
