/*****************************************************************************************************
Autor: Eliana Cepeda
Materia: Sistemas Operativos
Fecha: 15-08-2024
Materia: Sistemas Operativos
Tema: Gestión de memoria
	-Multiplicación de matrices usando punteros
	se solicitan los datos para las matrices a multiplicar y se muestra el resultado en pantalla
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

void multiplicar (int** m1, int** m2,int** result,int size){
	for (int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			*(*(result+i)+j)=0;
			for(int k=0; k<size; k++){
				*(*(result+i)+j)+= (*(*(m1+i)+k))* (*(*(m2+k)+j)); 
			}
		}
	}	 

}


int main(int argc, char* argv[]){
	if(argc<2){
		printf("\nArgumentos invalidos: $ejecutable size\n");
		exit(0);	
	}
	int size = atoi(argv[1]);
	int** mA =calloc(size, sizeof(int*));
	int** mB =calloc(size, sizeof(int*));    
	int** mC =calloc(size, sizeof(int*));  
  
	/* Incialiación*/
	for(int i=0; i<size; i++){
		*(mA+i) = calloc(size, sizeof(int));
		*(mB+i) = calloc(size, sizeof(int));
		*(mC+i) = calloc(size, sizeof(int));		
	}


	/* Solicitamos los datos de la matriz 1 y 2 */
	printf("Llene la primera matriz\nMtariz 1:\n");
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			printf("m1[%d][%d]: ",i,j);
			scanf(" %d",(*(mA+i)+j));
		}
	}
	
	printf("Llene la segunda matriz\nMtariz 2:\n");
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			printf("m2[%d][%d]: ",i,j);
			scanf(" %d",(*(mB+i)+j));
		}
	}
	
		
	/* impresion*/
	multiplicar(mA,mB,mC,size);
	printf("Matriz 1\n");
	imprimir (mA,atoi(argv[1]));
	printf("Matriz 2\n");
	imprimir (mB,atoi(argv[1]));
	printf("Resultado de m1 x m2\n");
	imprimir (mC,atoi(argv[1]));
    

	printf(" \nFin del programa\n");
	return 0;
}

