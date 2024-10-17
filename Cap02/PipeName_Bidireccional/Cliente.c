/*******************************************************************
Autor: Eliana Cepeda
Materia: Sistemas Operativos
Fecha: 3-Octubre-2024
Tema: Comunicación uni-direccional
      -PipeName

        ******Cliente*******

Descripción: 
En este programa se implementa un cliente, el cual se comunica de manera bi-direccional con el Servidor
a través de un PipeName llamado FIFO_FILE, el cual se crea temporalmente en "/tmp/fifo_twoway"
El cliente envía mensajes al servidor, recibiendo una respuesta del servidor con el mensaje escrito al revés.
La comunicación finaliza cuando el cliente envia el mensaje "end"
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>


#define FIFO_FILE "/tmp/fifo_twoway"


int main() {
   int fd; // descriptor del fichero
   int end_process; //indicador de terminación del proceso
   int stringlen; // cantidad de caracteres que transmite
   int read_bytes; //cantidad de caracteres leidos
   char readbuf[80];// bus de lectura con capacidad de 80
   char end_str[5]; // cadena de finalización, en este caso "fin"

    printf("\n  \t\t >>>>>>>>>>> Inicio cliente <<<<<<<<<<<<<< \n");

   printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");
   fd = open(FIFO_FILE, O_CREAT|O_RDWR); // permisos del pipe
   strcpy(end_str, "end"); // cadena de finalización 
   
   while (1) { //bucle infinito hasta que recibe la cadena de finalización y acaba el proceso
      printf("Enter string: ");
      fgets(readbuf, sizeof(readbuf), stdin);
      stringlen = strlen(readbuf);
      readbuf[stringlen - 1] = '\0';
      end_process = strcmp(readbuf, end_str);
      
      //printf("end_process is %d\n", end_process);
      if (end_process != 0) {
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         read_bytes = read(fd, readbuf, sizeof(readbuf));
         readbuf[read_bytes] = '\0';
         printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      } else {
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         close(fd);
         break;
      }
   }
   return 0;
}