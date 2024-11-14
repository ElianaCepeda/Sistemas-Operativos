CC = gcc
CFLAGS = -Wall -g
TARGETS = publicador suscriptor sistema
OBJ_DIR = build

all: $(TARGETS)

# Define rules for each target, with utils.h as a dependency for header functions
publicador: $(OBJ_DIR)/publicador.o
	$(CC) $(CFLAGS) -o publicador $(OBJ_DIR)/publicador.o

suscriptor: $(OBJ_DIR)/suscriptor.o
	$(CC) $(CFLAGS) -o suscriptor $(OBJ_DIR)/suscriptor.o

sistema: $(OBJ_DIR)/sistema_comunicacion.o
	$(CC) $(CFLAGS) -o sistema $(OBJ_DIR)/sistema_comunicacion.o

# General rule to compile source files into object files
$(OBJ_DIR)/%.o: src/%.c src/utils.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure the build directory exists before compiling
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(TARGETS) $(OBJ_DIR)/*.o
	rm -f pipePSC pipeSSC
	rm -rf $(OBJ_DIR)
