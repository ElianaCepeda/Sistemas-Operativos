/*****************************************************************************************************
Autor: Eliana Cepeda
Materia: Sistemas Operativos
Fecha: 15-08-2024
Materia: Sistemas Operativos
Tema: 	Gestión de memoria
	Creamos, inicializamos e imprimios 3 matrices
*****************************************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void imprimir (int** matriz, int size){
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++)
			printf("%d ",matriz[i][j]);
		printf("\n");		
	} 
	printf("_________________\n");
}


int main(int argc, char* argv[]){
	if(argc<2){
		printf("\nArgumentos invalidos: $ejecutable size\n");
		exit(0);	
	}
	int size = atoi(argv[1]);
	int** mA =NULL;
	int** mB =NULL;    
	int** mC =NULL;  
  
	/* Incialiación*/
	mA =calloc(size, sizeof(int*));
	mB =calloc(size, sizeof(int*));    
	mC =calloc(size, sizeof(int*));

	for(int i=0; i<size; i++){
		*(mA+i) = calloc(size, sizeof(int));
		*(mB+i) = calloc(size, sizeof(int));
		*(mC+i) = calloc(size, sizeof(int));		
	}


	/* Inicializamos las matrices con algun valor */
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			*(*(mA+i)+j)=0;
			*(*(mB+i)+j)=1;
			*(*(mC+i)+j)=2;
		}
	}
	
	
		
	/* impresion*/
	printf("Matriz 1\n");
	imprimir (mA,atoi(argv[1]));
	printf("Matriz 2\n");
	imprimir (mB,atoi(argv[1]));
	printf("Matriz 3 \n");
	imprimir (mC,atoi(argv[1]));
    

	printf(" \nFin del programa\n");
	return 0;
}

