/*******************************************************************
Autor: Eliana Cepeda
Materia: Sistemas Operativos
Fecha: 3-Octubre-2024
Tema: Comunicación uni-direccional
      -PipeName

        ******Cliente*******
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_FILE "MYFIFO"

 int main() {
    int fd; //descriptor del fichero
    int end_process; //indicador de terminación del proceso
    int stringlen; // cantidad de caracteres que transmite
    char readbuf[80]; // bus de lectura con capacidad de 80
    char end_str[5]; // cadena de finalización, en este caso "fin"

    printf("\n  \t\t >>>>>>>>>>> Inicio cliente <<<<<<<<<<<<<< \n");

    printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n"); //imprime las indicaciones de como funciona el pipe
    fd = open(FIFO_FILE, O_CREAT|O_WRONLY); // permisos del Pipe
    strcpy(end_str, "fin"); // cadena de finalización
 
    while (1) { // bucle infinito hasta que recibe la cadena de finalización y se muere el proceso
       printf("Enter string: "); 
       fgets(readbuf, sizeof(readbuf), stdin); // escribe la cadena que pone el cliente en el bus
       stringlen = strlen(readbuf); // cantidad de caracteres 
       readbuf[stringlen - 1] = '\0'; // adiciona signo de cambio de linea
       end_process = strcmp(readbuf, end_str); // compara si la cadena es "fin" 
 
       // valida si la cadena es "fin" para matar el proceso  
       if (end_process != 0) {
         // si no lo es escribe la cadena en el Pipe
          write(fd, readbuf, strlen(readbuf));
          printf("Sent string: \"%s\" and string length is %d\n", readbuf, (int)    strlen(readbuf)); 
       } else {
         // si lo es escribe la cadena en el Pipe y luego cierra el fichero
          write(fd, readbuf, strlen(readbuf));
          printf("Sent string: \"%s\" and string length is %d\n", readbuf, (int)    strlen(readbuf));
          close(fd);
          break;
       }
    }
    return 0;
 }