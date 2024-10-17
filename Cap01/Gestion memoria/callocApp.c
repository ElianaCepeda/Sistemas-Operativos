/*******************************************************************
Autor: Eliana Cepeda
Materia: Sistemas Operativos
Fecha: 8-Agosto-2024
Tema:	Asignación de memoria dinamica
	Asignación del tamaño en Bytes de almacenamiento
	Uso de función calloc( Asigna memoria y la inicializa en 0
********************************************************************/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv[]){
	int *varptr1 = calloc(4, sizeof(int)); //Asigna memoria inicializada en 0
	int *varptr2 = calloc(1, sizeof(int[4]));
	int *varptr3 = calloc(4, sizeof *varptr3);
	

	if(varptr1 ){
		printf("Valores que asigna calloc\n");

		for(int i=0; i<4; i++)
			printf("varptr1[%d]\t == %d\n", i, varptr1[i]);

		//Inicializamos valores con for 
		for(int i=0; i<4; i++)
			varptr1[i]=i*i;

		printf("Valores inicializados con for\n");
		for(int i=0; i<4; i++)
			printf("varptr1[%d]\t == %d\n", i, varptr1[i]);
	}
	
	//Liberamos memoria
	printf("Se libera memoria reservada\n");
	free(varptr1);
	free(varptr2);
	free(varptr3);

	return 0;
}
