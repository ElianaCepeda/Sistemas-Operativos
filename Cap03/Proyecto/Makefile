CC = gcc
CFLAGS = -Wall -g
TARGETS = publicador suscriptor sistema
OBJ_DIR = build

all: $(TARGETS)

# Define rules for each target, ensuring utils.o is linked where needed
publicador: $(OBJ_DIR)/publicador.o $(OBJ_DIR)/utils.o
	$(CC) $(CFLAGS) -o publicador $(OBJ_DIR)/publicador.o $(OBJ_DIR)/utils.o

suscriptor: $(OBJ_DIR)/suscriptor.o $(OBJ_DIR)/utils.o
	$(CC) $(CFLAGS) -o suscriptor $(OBJ_DIR)/suscriptor.o $(OBJ_DIR)/utils.o

sistema: $(OBJ_DIR)/sistema_comunicacion.o $(OBJ_DIR)/SCBiblioteca.o $(OBJ_DIR)/utils.o
	$(CC) $(CFLAGS) -o sistema $(OBJ_DIR)/sistema_comunicacion.o $(OBJ_DIR)/SCBiblioteca.o $(OBJ_DIR)/utils.o

# General rule to compile source files into object files
$(OBJ_DIR)/%.o: src/%.c src/utils.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile SCBiblioteca.c specifically
$(OBJ_DIR)/SCBiblioteca.o: src/SCBiblioteca.c src/SCBiblioteca.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c src/SCBiblioteca.c -o $(OBJ_DIR)/SCBiblioteca.o

# Compile utils.c specifically
$(OBJ_DIR)/utils.o: src/utils.c src/utils.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c src/utils.c -o $(OBJ_DIR)/utils.o

# Ensure the build directory exists before compiling
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(TARGETS) $(OBJ_DIR)/*.o
	rm -f pipePSC pipeSSC
	rm -rf $(OBJ_DIR)