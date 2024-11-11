#!/usr/bin/perl
#************************************************************************************************
# Fecha: 31/10/2024
# Autor: Daniel Castro-ELiana Cepeda-Maria Paula Rodriguez- Daniel Gonzalez
# Materia: Sistemas Operativos
# Tema: Taller 6 - Evaluación de rendimiento
# Descripción:
#     Este programa realiza la multiplicación de matrices cuadradas N×N empleando el modelo
#     de hilos POSIX (Pthreads) y evalúa el tiempo de ejecución del algoritmo clásico de
#     multiplicación de matrices distribuyendo el trabajo en múltiples hilos
#************************************************************************************************/

use strict;
use warnings;

# Ruta del directorio de trabajo actual
my $Path = `pwd`;
chomp($Path);

# Nombres de los ejecutables que se van a ejecutar
my @Nombres_Ejecutables = ("transpuesta");

# Tamaños de las matrices a evaluar
my @Size_Matriz = (600, 800, 1000, 1800);

# Número de hilos a utilizar en la ejecución
my @Num_Hilos = (1,2,4);

# Número de repeticiones para cada combinación de tamaño de matriz y número de hilos
my $Repeticiones = 30;

# Nombre del archivo de salida
my $output_file = "$Path/resultados_Transpuesta.dat";

# Abre el archivo de salida para escribir los resultados
open(my $out_fh, '>', $output_file) or die "No se pudo abrir el archivo $output_file: $!";

# Escribe el encabezado de la tabla en el archivo de salida
print $out_fh "\t\t";  # Espacio para la primera columna
foreach my $hilo (@Num_Hilos) {
    print $out_fh "Hilos: $hilo\t\t";
}
print $out_fh "\n";

# Itera sobre cada tamaño de matriz
foreach my $size (@Size_Matriz) {
    for (my $i = 0; $i < $Repeticiones; $i++) {
        print $out_fh "$size\t\t";  # Escribe el tamaño de la matriz en la primera columna

        # Itera sobre el número de hilos
        foreach my $hilo (@Num_Hilos) {
            my $output = `$Path/$Nombres_Ejecutables[0] $size $hilo`;

            # Verifica si el valor de tiempo está en la salida (asumimos un solo valor de tiempo en ms)
            if ($output =~ /\b(\d+(\.\d+)?)\b/) {
                printf $out_fh "%.d   \t\t", $1;
            } else {
                die "No se encontró un valor de tiempo en la salida de $Nombres_Ejecutables[0] con tamaño $size y $hilo hilos.";
            }
        }
        print $out_fh "\n";  # Nueva línea para la siguiente ejecución
    }
        print $out_fh "\n";  # inserta un espacio entre los datos de un tamaño y otro
}

# Cierra el archivo de salida
close($out_fh);
print "Resultados guardados en $output_file\n";
