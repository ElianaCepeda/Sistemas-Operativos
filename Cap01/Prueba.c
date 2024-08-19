/*******************************************************************
Autor: Eliana Cepeda
Materia: Sistemas Operativos
Fecha: 8-Agosto-2024
Tema: Punteros
  - Suma de dos valores
********************************************************************/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv[]){
	int var = 480;
	int *varptr= &var;
	int **doubleptr= &varptr;
	printf("valor de la Variable \t\t=%d\n", var);
	printf("Valor del puntero     \t\t=%d\n",*varptr );
	printf("valor del puntero doble\t\t=%d\n", **doubleptr);
	
	printf("Direccion de la variable \t=%p\n", &var );
	
	printf("Direccion del puntero \t\t=%p\n", &varptr);
	printf("Valor en el puntero \t\t=%p\n", varptr);

	printf("Direccion del puntero doble\t=%p\n", *doubleptr);
	printf("Valor en el puntero doble \t=%p\n", doubleptr);

	return 0;
}
