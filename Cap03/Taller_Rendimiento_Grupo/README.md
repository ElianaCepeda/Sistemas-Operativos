# Benchmarking de Multiplicación de Matrices con Hilos POSIX (Pthreads)

Este proyecto evalúa el rendimiento de dos algoritmos de multiplicación de matrices cuadradas (`clásico` y `transpuesta`) implementados en C con hilos POSIX (`Pthreads`). Ambos programas ejecutan multiplicaciones de matrices de varios tamaños con diferentes cantidades de hilos y miden el tiempo de ejecución para cada combinación.

## Estructura del Proyecto

El repositorio contiene los siguientes archivos:

### Código en C

1. **`mm_clasico.c`**: Programa en C que implementa el algoritmo clásico de multiplicación de matrices cuadradas usando hilos. Utiliza las funciones definidas en `mm_clasicoBiblioteca.c` para realizar la multiplicación.
2. **`mm_transpuesta.c`**: Programa en C que implementa el algoritmo de multiplicación de matrices cuadradas transpuestas usando hilos. Utiliza las funciones definidas en `mm_transpuestaBiblioteca.c`.
3. **`mm_clasicoBiblioteca.c` y `mm_clasicoBiblioteca.h`**: Biblioteca de funciones y variables globales para el programa `mm_clasico.c`.
4. **`mm_transpuestaBiblioteca.c` y `mm_transpuestaBiblioteca.h`**: Biblioteca de funciones y variables globales para el programa `mm_transpuesta.c`.

### Scripts de Benchmark en Perl

5. **`lanza_clasico.pl`**: Script en Perl que ejecuta el programa `clasico` varias veces con diferentes tamaños de matrices y números de hilos, y almacena los resultados en un archivo de salida.
6. **`lanza_transpuesta.pl`**: Script en Perl que ejecuta el programa `transpuesta` varias veces con diferentes configuraciones y guarda los resultados en un archivo de salida.

### Archivos de Salida

7. **`resultados_Clasico.dat`**: Archivo de salida que contiene los resultados de las ejecuciones del programa `clasico`.
8. **`resultados_Transpuesta.dat`**: Archivo de salida que contiene los resultados de las ejecuciones del programa `transpuesta`.

## Requisitos Previos

1. **Compilador GCC**: Se necesita `gcc` para compilar los programas en C.
2. **Perl**: Se necesita Perl para ejecutar los scripts `lanza_clasico.pl` y `lanza_transpuesta.pl`.

## Compilación

Para compilar los programas `clasico` y `transpuesta`, ejecuta los siguientes comandos:

```bash
gcc mm_clasico.c mm_clasicoBiblioteca.c -o clasico -lpthread
gcc mm_transpuesta.c mm_transpuestaBiblioteca.c -o transpuesta -lpthread
```

Esto generará los ejecutables `clasico` y `transpuesta` en el directorio de trabajo.

## Ejecución de los Scripts de Benchmark

Cada script de Perl (`lanza_clasico.pl` y `lanza_transpuesta.pl`) ejecuta múltiples pruebas con los programas correspondientes (`clasico` o `transpuesta`). Los scripts varían el tamaño de la matriz y el número de hilos, y guardan los tiempos de ejecución en archivos de salida (`resultados_Clasico.dat` y `resultados_Transpuesta.dat`).

### Ejecución de `lanza_clasico.pl`

Ejecuta el script de Perl para el programa clásico con el siguiente comando:

```bash
perl lanza_clasico.pl
```

Este comando generará el archivo `resultados_Clasico.dat` con los tiempos de ejecución de las diferentes combinaciones de tamaño de matriz y número de hilos.

### Ejecución de `lanza_transpuesta.pl`

Ejecuta el script de Perl para el programa transpuesto con el siguiente comando:

```bash
perl lanza_transpuesta.pl
```

Este comando generará el archivo `resultados_Transpuesta.dat` con los tiempos de ejecución de las diferentes combinaciones de tamaño de matriz y número de hilos.

## Estructura de los Archivos de Salida

Los archivos de salida (`resultados_Clasico.dat` y `resultados_Transpuesta.dat`) tienen un formato tabular, donde cada columna representa el tiempo de ejecución (en microsegundos) para una combinación específica de tamaño de matriz y número de hilos.

Ejemplo de un archivo de salida:

```
              Hilos: 1      Hilos: 2      Hilos: 4
600           123456        98765         87654
800           234567        87654         76543
...
```

Cada fila representa el tamaño de la matriz, y cada columna representa el tiempo promedio de ejecución con el número de hilos especificado.

## Interpretación de Resultados

Estos archivos de resultados se pueden analizar para observar cómo cambia el rendimiento de la multiplicación de matrices a medida que se aumenta el número de hilos y el tamaño de la matriz. Esto permite realizar comparaciones entre el algoritmo clásico y el de transpuesta para evaluar cuál es más eficiente en diferentes configuraciones de hardware y tamaño de matriz.
