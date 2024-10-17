# Makefile para compilar el programa del Tercer Taller de Sistemas Operativos
# Fecha: 12/10/2024
# Autores: Daniel Castro, María Paula Rodríguez, Eliana Cepeda, Daniel Gómez
# Materia: Sistemas Operativos
# Tema: Tercer Taller - fork()

CC = gcc
CFLAGS = -Wall -g
TARGET = ejecutable
OBJ = taller3.o funcs.o

# Archivos de entrada y sus tamaños
N1 = 30
ARCHIVO1 = archivo00.txt
N2 = 20
ARCHIVO2 = archivo01.txt

all: $(TARGET) run

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

taller3.o: taller3.c taller3.h
	$(CC) $(CFLAGS) -c taller3.c

funcs.o: funcs.c taller3.h
	$(CC) $(CFLAGS) -c funcs.c

# Regla para ejecutar el programa con los argumentos predefinidos
run:
	./$(TARGET) $(N1) $(ARCHIVO1) $(N2) $(ARCHIVO2)

# Regla para limpiar los archivos objeto y el ejecutable
clean:
	rm -f $(OBJ) $(TARGET)
