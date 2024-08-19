/*******************************************************************
Autor: Eliana Cepeda
Materia: Sistemas Operativos
Fecha: 8-Agosto-2024
Tema: Asignación de memoria dinamica
	Re-Asignación del tamaño en Bytes de almacenamiento
	Uso funcion realloc
********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*Función para imprimir el tamaño reasignado**/
void Info_Almacenamiento(const int *siguiente, const int *anterior, int valores){

	if(siguiente){
		printf("%s ubicación = %p. Tamaño Reservado: %d(d bytes)\n. ", (siguiente!=anterior ? "Nuevo" :"Anterior"), (void*)siguiente, valores*sizeof(int));
	}

}

int main(int argc, char **argv[]){
	const int patron[] = {1,2,3,4,5,6,7,8};
	const int patron_size = sizeof (patron)/ sizeof(int);
	int *siguiente = NULL, *anterior = NULL;
	
	siguiente = malloc(patron_size*sizeof(int));
	if(siguiente != NULL){ //Asigna el vector
		memcpy(siguiente, patron, sizeof (patron)); //llena el vector
		Info_Almacenamiento (siguiente, anterior, patron_size);
	}
	// Reasigna los valores del vector en el siguiente ciclo
	const int realloc_size[] = {10,20,30,40,50,60,70,80};	
	for(int i =0; i!= sizeof realloc_size/ sizeof(int); i++){
		if(siguiente = (int*) realloc(anterior =siguiente, realloc_size[i]*sizeof(int))){
	
			Info_Almacenamiento (siguiente, anterior, realloc_size[i]);
		}else{
			printf("Se libera memoria reservada\n");
			free(siguiente);
		}
	}

	return 0;
}
