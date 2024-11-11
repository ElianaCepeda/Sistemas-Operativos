/************************************************************************************************
Fecha: 10/11/2024
Autor: Daniel Castro, María Paula Rodríguez, Eliana Cepeda, Daniel Gomez
Materia: Sistemas Operativos
Tema: Suscriptor de Noticias a través de Tuberías
Descripción:
    El sistema implementado gestiona la transmisión y suscripción de noticias a través de tuberías 
    en un sistema de comunicación entre un publicador y suscriptores. El publicador envía noticias 
    a través de un canal común, y los suscriptores se registran para recibir noticias sobre temas 
    específicos. El sistema gestiona la creación de tuberías y la transmisión de noticias de manera 
    eficiente utilizando técnicas de selección no bloqueante.
************************************************************************************************
            SISTEMA_COMUNICACIÓN
************************************************************************************************/
/* Importación de librerías necesarias */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/select.h>
#include <time.h>

#define BUFFER_SIZE 1024 // Tamaño del buffer para la comunicación
#define MAX_SUSCRIPTORES 100 // Cantidad máxima de suscriptores registrados
#define MAX_NOTICIAS 100 // Cantidad máxima de noticias almacenadas

// Estructura para representar una noticia, contiene el tipo (tema) y su contenido
typedef struct {
    char tipo; // Tipo de tema, por ejemplo, 'A', 'E', etc.
    char contenido[BUFFER_SIZE]; // Contenido de la noticia
} Noticia;

// Estructura para representar un suscriptor, con sus temas de interés y el nombre de su tubería
typedef struct {
    char topics[BUFFER_SIZE]; // Lista de temas en los que está interesado el suscriptor
    char pipe_name[BUFFER_SIZE]; // Nombre de la tubería única del suscriptor
} Suscriptor;

Suscriptor suscriptores[MAX_SUSCRIPTORES]; // Arreglo para almacenar suscriptores registrados
Noticia noticias[MAX_NOTICIAS]; // Arreglo para almacenar las noticias recibidas
int num_suscriptores = 0; // Contador de suscriptores registrados
int num_noticias = 0; // Contador de noticias almacenadas
time_t last_news_time; // Timestamp de la última noticia recibida

void almacenar_noticia(const char *noticia_text) {
    if (num_noticias >= MAX_NOTICIAS) { // Verifica si el almacenamiento de noticias está lleno
        fprintf(stderr, "News storage is full, cannot store more news.\n");
        return;
    }
    noticias[num_noticias].tipo = noticia_text[0]; // El tipo de tema es el primer carácter
    strncpy(noticias[num_noticias].contenido, noticia_text, BUFFER_SIZE); // Copia el contenido de la noticia
    num_noticias++; // Incrementa el contador de noticias almacenadas
    last_news_time = time(NULL); // Actualiza el timestamp de la última noticia
}

void enviar_noticia_a_suscriptor(const Suscriptor *suscriptor, const Noticia *noticia) {
    if (strchr(suscriptor->topics, noticia->tipo)) { // Compara el tipo de noticia con los temas de interés
        int fd = open(suscriptor->pipe_name, O_WRONLY | O_NONBLOCK); // Abre la tubería del suscriptor en modo no bloqueante
        if (fd != -1) {
            write(fd, noticia->contenido, strlen(noticia->contenido)); // Envía el contenido de la noticia
            close(fd); // Cierra la tubería después de escribir
        } else {
            perror("Error opening subscriber pipe"); // Muestra error si no se puede abrir la tubería
        }
    }
}

void registrar_suscriptor(const char *message) {
    if (num_suscriptores >= MAX_SUSCRIPTORES) { // Verifica si se ha alcanzado el límite de suscriptores
        fprintf(stderr, "Max subscribers reached.\n");
        return;
    }
    char *delimiter = strchr(message, ':'); // Busca el delimitador ':' para separar la tubería y los temas
    if (!delimiter) return;
    *delimiter = '\0'; // Termina el nombre de la tubería
    strncpy(suscriptores[num_suscriptores].pipe_name, message, BUFFER_SIZE); // Copia el nombre de la tubería
    // Normaliza los temas, eliminando los espacios para una comparación más sencilla
    char *topics = delimiter + 1;
    for (int i = 0, j = 0; topics[i] != '\0'; i++) {
        if (topics[i] != ' ') {
            suscriptores[num_suscriptores].topics[j++] = topics[i];
        }
    }
    printf("Subscriber registered: Pipe=%s Topics=%s\n",
           suscriptores[num_suscriptores].pipe_name,
           suscriptores[num_suscriptores].topics);

    // Enviar todas las noticias relevantes al nuevo suscriptor
    for (int i = 0; i < num_noticias; i++) {
        enviar_noticia_a_suscriptor(&suscriptores[num_suscriptores], &noticias[i]);
    }

    num_suscriptores++; // Incrementa el contador de suscriptores
}

void finalizar_transmision() {
    const char *end_message = "End of news broadcast.\n"; // Mensaje de fin de la transmisión
    printf("No new publishers detected. Broadcasting end of transmission.\n");
    for (int i = 0; i < num_suscriptores; i++) {
        int fd = open(suscriptores[i].pipe_name, O_WRONLY | O_NONBLOCK); // Abre la tubería del suscriptor
        if (fd != -1) {
            write(fd, end_message, strlen(end_message)); // Envía el mensaje de fin
            close(fd); // Cierra la tubería después de escribir
        } else {
            perror("Error opening subscriber pipe for final message"); // Muestra error si no se puede abrir la tubería
        }
    }
}

int main(int argc, char *argv[]) {
    char *pipePSC = NULL, *pipeSSC = NULL;
    int timeF = 0;

    // Procesa los argumentos de la línea de comandos para obtener las tuberías y el tiempo de espera
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            pipePSC = argv[++i]; // Tubería para el publicador
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            pipeSSC = argv[++i]; // Tubería para el suscriptor
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            timeF = atoi(argv[++i]); // Tiempo de espera para finalizar transmisión
        }
    }

    // Verifica que todos los parámetros sean correctos
    if (!pipePSC || !pipeSSC || timeF <= 0) {
        fprintf(stderr, "Usage: sistema -p pipePSC -s pipeSSC -t timeF\n");
        exit(EXIT_FAILURE);
    }

    mkfifo(pipePSC, 0666);  // Crea la tubería del publicador
    mkfifo(pipeSSC, 0666);  // Crea la tubería del suscriptor

    int fdPSC = open(pipePSC, O_RDONLY | O_NONBLOCK);  // Abre la tubería del publicador en modo no bloqueante
    int fdSSC = open(pipeSSC, O_RDONLY | O_NONBLOCK);  // Abre la tubería del suscriptor en modo no bloqueante

    if (fdPSC == -1 || fdSSC == -1) { // Verifica si las tuberías se abrieron correctamente
        perror("Error opening pipes");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE]; // Buffer para recibir los mensajes
    fd_set read_fds; // Conjunto de descriptores de archivo para la función select
    int max_fd = fdPSC > fdSSC ? fdPSC : fdSSC; // Determina el descriptor de archivo máximo

    struct timeval timeout = {timeF, 0}; // Timeout de la función select

    // Bucle principal que espera mensajes
    while (1) {
        FD_ZERO(&read_fds); // Inicializa el conjunto de descriptores
        FD_SET(fdPSC, &read_fds); // Añade el descriptor de la tubería del publicador
        FD_SET(fdSSC, &read_fds); // Añade el descriptor de la tubería del suscriptor

        int select_result = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

        if (select_result > 0) {
            if (FD_ISSET(fdPSC, &read_fds)) { // Si hay algo en la tubería del publicador
                int bytes_read = read(fdPSC, buffer, BUFFER_SIZE);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0'; // Asegura que la cadena esté bien terminada
                    almacenar_noticia(buffer); // Almacena la noticia recibida
                }
            }
            if (FD_ISSET(fdSSC, &read_fds)) { // Si hay algo en la tubería del suscriptor
                int bytes_read = read(fdSSC, buffer, BUFFER_SIZE);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0'; // Asegura que la cadena esté bien terminada
                    registrar_suscriptor(buffer); // Registra un nuevo suscriptor
                }
            }
        } else if (select_result == 0) { // Si ha expirado el tiempo de espera
            finalizar_transmision(); // Finaliza la transmisión
            break;
        } else {
            perror("Error in select"); // Si ocurrió un error en select
            break;
        }
    }

    // Limpia y cierra las tuberías
    close(fdPSC);
    close(fdSSC);
    unlink(pipePSC);
    unlink(pipeSSC);

    return 0;
}