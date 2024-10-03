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
    mknod(FIFO_FILE, S_IFIFO|0640, 0); // 2 lectura, 4 escritura
    strcpy(end, "fin");

    while(1) { // bucle infinito que solo sale si recibe "fin"
       fd = open(FIFO_FILE, O_RDONLY);
            read_bytes = read(fd, readbuf, sizeof(readbuf)); 
            readbuf[read_bytes] = '\0'; 
                printf("Received string: \"%s\" and length is %d\n", readbuf, (int)strlen    (readbuf));
       to_end = strcmp(readbuf, end);
       if (to_end == 0) {
          close(fd);
          break;
       }
    }
 }