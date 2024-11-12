/**************************************************************
		Pontificia Universidad Javeriana
	Autor: Daniel Castro, Maria Ruiz, Daniel Gonzalez, Eliana Cepeda
	Fecha: Noviembre 2024
	Materia: Sistemas Operativos
	Tema: Este proyecto implementa un sistema de distribución de noticias usando procesos en C 
		y comunicación mediante pipes nominales (FIFOs). El sistema permite que un publicador envíe 
		noticias a un sistema de comunicación, el cual las distribuye a los suscriptores interesados en temas específicos.
****************************************************************/
#include <stdio.h>              // Biblioteca estándar para funciones de entrada/salida
#include <stdlib.h>             // Biblioteca estándar para funciones generales de C
#include <string.h>             // Biblioteca para manejo de cadenas de caracteres
#include <unistd.h>             // Biblioteca POSIX para manejo de archivos y procesos
#include <fcntl.h>              // Biblioteca para control de archivos (flags como O_WRONLY, O_NONBLOCK)
#include <sys/stat.h>           // Biblioteca para manejo de archivos (permisos y tipos de archivos)
#include <errno.h>              // Biblioteca para manejo de errores
#include <sys/select.h>         // Biblioteca para monitorear múltiples descriptores de archivos
#include <time.h>               // Biblioteca para funciones y estructuras de tiempo

#define BUFFER_SIZE 1024        // Tamaño del buffer para lectura y escritura
#define MAX_SUSCRIPTORES 100    // Número máximo de suscriptores
#define MAX_NOTICIAS 100        // Número máximo de noticias almacenadas

// Estructura para almacenar una noticia
typedef struct {
    char tipo;                    // Tipo de noticia (e.g., 'A', 'E')
    char contenido[BUFFER_SIZE];  // Descripción de la noticia
} Noticia;

// Estructura para almacenar un suscriptor
typedef struct {
    char topics[BUFFER_SIZE];     // Temas de interés del suscriptor (e.g., "A E")
    char pipe_name[BUFFER_SIZE];  // Nombre del pipe único del suscriptor
} Suscriptor;

Suscriptor suscriptores[MAX_SUSCRIPTORES]; // Array de suscriptores
Noticia noticias[MAX_NOTICIAS];            // Array de noticias
int num_suscriptores = 0;                  // Número de suscriptores actuales
int num_noticias = 0;                      // Número de noticias almacenadas
time_t last_news_time;                     // Marca de tiempo de la última noticia recibida

// Almacena una noticia en el array `noticias`
void almacenar_noticia(const char *noticia_text) {
    if (num_noticias >= MAX_NOTICIAS) {    // Verifica si el almacenamiento está lleno
        fprintf(stderr, "News storage is full, cannot store more news.\n");
        return;
    }
    noticias[num_noticias].tipo = noticia_text[0];  // El tipo de noticia es el primer carácter
    strncpy(noticias[num_noticias].contenido, noticia_text, BUFFER_SIZE);  // Almacena el contenido de la noticia
    num_noticias++;                       // Incrementa el contador de noticias
    last_news_time = time(NULL);          // Actualiza el tiempo de la última noticia
}

// Envía una noticia a un suscriptor si coincide con sus temas de interés
void enviar_noticia_a_suscriptor(const Suscriptor *suscriptor, const Noticia *noticia) {
    if (strchr(suscriptor->topics, noticia->tipo)) {  // Verifica si el tema coincide
        int fd = open(suscriptor->pipe_name, O_WRONLY | O_NONBLOCK); // Abre el pipe del suscriptor
        if (fd != -1) {           // Si el pipe se abre correctamente, envía la noticia
            write(fd, noticia->contenido, strlen(noticia->contenido));
            close(fd);            // Cierra el pipe después de enviar la noticia
        } else {
            perror("Error opening subscriber pipe");  // Mensaje de error si no se puede abrir el pipe
        }
    }
}

// Registra un nuevo suscriptor a partir de un mensaje recibido
void registrar_suscriptor(const char *message) {
    if (num_suscriptores >= MAX_SUSCRIPTORES) {  // Verifica si se alcanzó el máximo de suscriptores
        fprintf(stderr, "Max subscribers reached.\n");
        return;
    }
    char *delimiter = strchr(message, ':');  // Busca el delimitador ":" en el mensaje
    if (!delimiter) return;                 // Si no se encuentra, termina la función
    *delimiter = '\0';                      // Separa el nombre del pipe y los temas
    strncpy(suscriptores[num_suscriptores].pipe_name, message, BUFFER_SIZE); // Almacena el nombre del pipe

    // Normaliza los temas eliminando espacios
    char *topics = delimiter + 1;
    for (int i = 0, j = 0; topics[i] != '\0'; i++) {
        if (topics[i] != ' ') {              // Solo guarda caracteres distintos de espacios
            suscriptores[num_suscriptores].topics[j++] = topics[i];
        }
    }
    printf("Subscriber registered: Pipe=%s Topics=%s\n",
           suscriptores[num_suscriptores].pipe_name,
           suscriptores[num_suscriptores].topics);

    // Envía todas las noticias relevantes al nuevo suscriptor
    for (int i = 0; i < num_noticias; i++) {
        enviar_noticia_a_suscriptor(&suscriptores[num_suscriptores], &noticias[i]);
    }

    num_suscriptores++;                      // Incrementa el contador de suscriptores
}

// Envía un mensaje de final de transmisión a todos los suscriptores
void finalizar_transmision() {
    const char *end_message = "End of news broadcast.\n";
    printf("No new publishers detected. Broadcasting end of transmission.\n");
    for (int i = 0; i < num_suscriptores; i++) {
        int fd = open(suscriptores[i].pipe_name, O_WRONLY | O_NONBLOCK); // Abre el pipe del suscriptor
        if (fd != -1) {
            write(fd, end_message, strlen(end_message));  // Envía el mensaje de fin de transmisión
            close(fd);            // Cierra el pipe después de enviar
        } else {
            perror("Error opening subscriber pipe for final message");  // Error si no se puede abrir el pipe
        }
    }
}

// Función principal del sistema de distribución de noticias
int main(int argc, char *argv[]) {
    char *pipePSC = NULL, *pipeSSC = NULL;  // Variables para almacenar los nombres de los pipes
    int timeF = 0;                          // Tiempo de inactividad para finalizar la transmisión

    // Procesa los argumentos de la línea de comandos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            pipePSC = argv[++i];
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            pipeSSC = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            timeF = atoi(argv[++i]);
        }
    }
    if (!pipePSC || !pipeSSC || timeF <= 0) { // Verifica que los parámetros necesarios estén presentes
        fprintf(stderr, "Usage: sistema -p pipePSC -s pipeSSC -t timeF\n");
        exit(EXIT_FAILURE);
    }

    mkfifo(pipePSC, 0666);  // Crea el pipe de publicador
    mkfifo(pipeSSC, 0666);  // Crea el pipe de suscriptor

    int fdPSC = open(pipePSC, O_RDONLY | O_NONBLOCK);  // Abre `pipePSC` en modo no bloqueante
    int fdSSC = open(pipeSSC, O_RDONLY | O_NONBLOCK);  // Abre `pipeSSC` en modo no bloqueante

    if (fdPSC == -1 || fdSSC == -1) {  // Verifica si los pipes se abrieron correctamente
        perror("Error opening pipes");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];          // Buffer para leer datos de los pipes
    fd_set read_fds;                   // Conjunto de descriptores para `select`
    int max_fd = (fdPSC > fdSSC ? fdPSC : fdSSC) + 1; // Calcula el descriptor máximo para `select`

    last_news_time = time(NULL);       // Inicializa el tiempo de la última noticia
    printf(">>> SYSTEM STARTED <<<\n");

    while (1) {
        FD_ZERO(&read_fds);            // Inicializa el conjunto de descriptores de lectura
        FD_SET(fdPSC, &read_fds);      // Agrega `fdPSC` al conjunto
        FD_SET(fdSSC, &read_fds);      // Agrega `fdSSC` al conjunto

        struct timeval timeout;
        timeout.tv_sec = 1;            // Tiempo de espera de 1 segundo
        timeout.tv_usec = 0;

        if (select(max_fd, &read_fds, NULL, NULL, &timeout) > 0) { // Monitorea los descriptores de archivo
            if (FD_ISSET(fdPSC, &read_fds)) {  // Verifica si hay datos en `fdPSC`
                int bytes_read = read(fdPSC, buffer, sizeof(buffer) - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';   // Agrega terminador nulo a la cadena leída
                    printf("News received from publisher: %s\n", buffer);
                    last_news_time = time(NULL); // Actualiza el tiempo de la última noticia
                    almacenar_noticia(buffer);   // Almacena la noticia

                    // Distribuye la noticia a todos los suscriptores
                    for (int i = 0; i < num_suscriptores; i++) {
                        enviar_noticia_a_suscriptor(&suscriptores[i], &noticias[num_noticias - 1]);
                    }
                }
            }
            if (FD_ISSET(fdSSC, &read_fds)) {  // Verifica si hay datos en `fdSSC`
                int bytes_read = read(fdSSC, buffer, sizeof(buffer) - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    registrar_suscriptor(buffer);  // Registra al suscriptor y envía noticias relevantes
                }
            }
        }

        // Verifica inactividad en `pipePSC` (pipe de publicador)
        if (difftime(time(NULL), last_news_time) >= timeF) {
            finalizar_transmision();   // Finaliza la transmisión si hay inactividad
            break;
        }
    }

    close(fdPSC);                      // Cierra `fdPSC`
    close(fdSSC);                      // Cierra `fdSSC`
    unlink(pipePSC);                   // Elimina `pipePSC`
    unlink(pipeSSC);                   // Elimina `pipeSSC`
    return 0;
}
