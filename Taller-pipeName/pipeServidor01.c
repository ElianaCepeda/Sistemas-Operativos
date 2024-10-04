/*******************************************************************
Autor: Eliana Cepeda
Materia: Sistemas Operativos
Fecha: 3-Octubre-2024
Tema: Comunicación uni-direccional
      -PipeName

        ******Servidor*****
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


#define FIFO_FILE "MYFIFO"


 int main() {
    int fd; //descriptor del fichero
    char readbuf[80]; // bus de tipo char con capacidad de 80 
    char end[10]; // mensaje de finalización en este caso "fin"
    int to_end; // identificador de la cadena de finalización
    int read_bytes; //cantidad de bytes que transmito
    
    printf("\n  \t\t >>>>>>>>>>> Inicio servidor <<<<<<<<<<<<<< \n");

    /* Create the FIFO if it does not exist */
    mknod(FIFO_FILE, S_IFIFO|0640, 0); // codigos para permisos 2 lectura, 4 escritura y se suman si son ambos permisos
    strcpy(end, "fin"); // recibe la cadena que indica que hay que cerrarlo

    while(1) { // bucle infinito que solo sale si recibe "fin"
       fd = open(FIFO_FILE, O_RDONLY); // indica que el FIFO solo de abre en lectura
            read_bytes = read(fd, readbuf, sizeof(readbuf));  //guarda los datos que recibe en el bus
            readbuf[read_bytes] = '\0'; // le agrega el cambio de linea para imprimirlo lindo 
                printf("Received string: \"%s\" and length is %d\n", readbuf, (int)strlen    (readbuf)); // imprime la cadena que recibió
       to_end = strcmp(readbuf, end); // compara si la cadena recibida es "fin"
       // verifica si la cadena era "fin" y cierra el fichero
       if (to_end == 0) {
          close(fd);
          break;
       }

       // no se esta eliminando el fichero
    }
 }