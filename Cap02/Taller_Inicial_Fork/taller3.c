/************************************************************************************************
Fecha: 12/10/2024
Autores: Daniel Castro, María Paula Rodríguez, Eliana Cepeda, Daniel Gómez
Materia: Sistemas Operativos
Tema: Tercer Taller - fork()

Descripción:
- Este programa implementa un sistema de procesamiento de arreglos enteros leídos desde dos archivos
  de texto. Recibe como argumentos de línea de comandos dos archivos de texto y dos enteros que
  representan el tamaño de los arreglos contenidos en dichos archivos.
  El programa realiza cálculos de suma sobre cada arreglo utilizando procesos hijos, creados mediante
  dos llamadas a `fork()`.

- Estructura del programa:
  1. Se crean tres procesos adicionales al proceso padre:
     - **Primer Hijo:** Lee y suma los valores del primer archivo de texto (arreglo1).
     - **Segundo Hijo:** Lee y suma los valores del segundo archivo de texto (arreglo2).
     - **Gran Hijo:** Es creado por el primer hijo y se encarga de sumar los resultados parciales
       de ambos arreglos (resultado de arreglo1 y arreglo2) y enviar el resultado al proceso padre.
  2. La comunicación entre los procesos se lleva a cabo mediante un arreglo bidimensional de pipes.
     Cada pipe es un canal unidireccional que permite la transferencia de datos entre procesos.

- El proceso principal (padre) coordina la lectura de los resultados parciales desde los pipes,
  espera a que todos los procesos terminen usando `wait()`, y finalmente muestra el resultado total
  de la suma de ambos arreglos.

************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "taller3.h"

// Función principal que maneja el flujo general del programa y la creación de procesos.
int main(int argc, char *argv[]) {
    // Validación de los argumentos de entrada. Se espera: N1 archivo00 N2 archivo01
    if (argc != 5) {
        fprintf(stderr, "Uso incorrecto: %s N1 archivo00 N2 archivo01\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Se extraen los tamaños de los arreglos y los nombres de los archivos de los argumentos.
    int N1 = atoi(argv[1]);
    char *archivo00 = argv[2];
    int N2 = atoi(argv[3]);
    char *archivo01 = argv[4];

    // Se declaran los punteros para almacenar los arreglos que se leerán desde los archivos.
    int *arreglo1 = NULL, *arreglo2 = NULL;

    // Se leen los datos de los archivos y se almacenan en los arreglos.
    leer_arreglo(archivo00, &N1, &arreglo1);  // Lee el primer archivo en arreglo1.
    leer_arreglo(archivo01, &N2, &arreglo2);  // Lee el segundo archivo en arreglo2.

    // Se crean tres pipes para la comunicación entre procesos: uno para cada proceso.
    int (*pipes)[2] = malloc(3 * sizeof(int[2]));
    for (int i = 0; i < 3; i++) {
        // Se verifica si la creación del pipe ha fallado y se maneja el error.
        if (pipe(pipes[i]) == -1) {
            perror("Error al crear el pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Primera llamada a fork() para crear el primer conjunto de procesos.
    pid_t primerfork = fork();
    if (primerfork < 0) {
        perror("Error en la ejecución de fork()");
        exit(EXIT_FAILURE);
    }

    // Segunda llamada a fork() para crear el segundo conjunto de procesos.
    pid_t segundofork = fork();
    if (segundofork < 0) {
        perror("Error en la ejecución de fork()");
        exit(EXIT_FAILURE);
    }

    // Segundo Hijo: se encarga de sumar los elementos de arreglo2 (archivo01).
    if (primerfork > 0 && segundofork == 0) {
        int sumaB = suma(arreglo2, N2);  // Realiza la suma del segundo arreglo.
        write(pipes[1][1], &sumaB, sizeof(sumaB));  // Envía el resultado al proceso padre a través del pipe.
        close(pipes[1][1]);  // Cierra el extremo de escritura del pipe.
        printf("SegundoHijo: [%d] Suma del archivo %s = %d\n", getpid(), archivo01, sumaB);
        exit(EXIT_SUCCESS);  // Finaliza el proceso del segundo hijo.
    }

    // Gran Hijo: creado por el primer hijo, suma los elementos de arreglo1 (archivo00).
    else if (primerfork == 0 && segundofork == 0) {
        int sumaA = suma(arreglo1, N1);  // Realiza la suma del primer arreglo.
        write(pipes[0][1], &sumaA, sizeof(sumaA));  // Envía el resultado al proceso padre a través del pipe.
        close(pipes[0][1]);  // Cierra el extremo de escritura del pipe.
        printf("GranHijo: [%d] Suma del archivo %s = %d\n", getpid(), archivo00, sumaA);
        exit(EXIT_SUCCESS);  // Finaliza el proceso del gran hijo.
    }

    // Primer Hijo: recibe las sumas parciales de los otros hijos y calcula la suma total.
    else if (primerfork == 0 && segundofork > 0) {
        int sumaA, sumaB;
        read(pipes[0][0], &sumaA, sizeof(sumaA));  // Lee la suma parcial del Gran Hijo.
        read(pipes[1][0], &sumaB, sizeof(sumaB));  // Lee la suma parcial del Segundo Hijo.

        int suma_total = sumaA + sumaB;  // Calcula la suma total de ambos arreglos.

        // Envía la suma total al proceso Padre mediante el pipe.
        write(pipes[2][1], &suma_total, sizeof(suma_total));
        close(pipes[2][1]);  // Cierra el extremo de escritura del pipe.
        printf("PrimerHijo: [%d] Suma total de ambos archivos = %d\n", getpid(), suma_total);
        exit(EXIT_SUCCESS);  // Finaliza el proceso del primer hijo.
    }

    // Proceso Padre: recibe la suma total desde el primer hijo y muestra el resultado.
    else if (primerfork > 0 && segundofork > 0) {
        // Espera a que los procesos hijo y gran hijo terminen de ejecutar.
        wait(NULL);  // Espera al Primer Hijo.
        wait(NULL);  // Espera al Segundo Hijo.
        wait(NULL);  // Espera al Gran Hijo.

        int suma_total_recibida;  // Variable para almacenar la suma total recibida.

        // Lee la suma total enviada por el Primer Hijo a través del pipe.
        read(pipes[2][0], &suma_total_recibida, sizeof(suma_total_recibida));
        printf("Padre: [%d] Suma total recibida del Primer Hijo = %d\n", getpid(), suma_total_recibida);
    }

    // Cierre de todos los extremos de los pipes después de la comunicación.
    for (int i = 0; i < 3; i++) {
        close(pipes[i][0]);  // Cierra el extremo de lectura.
        close(pipes[i][1]);  // Cierra el extremo de escritura.
    }

    // Liberación de la memoria asignada dinámicamente para los arreglos y pipes.
    free(arreglo1);
    free(arreglo2);
    free(pipes);

    return EXIT_SUCCESS;  // Finaliza la ejecución del proceso padre con éxito.
}