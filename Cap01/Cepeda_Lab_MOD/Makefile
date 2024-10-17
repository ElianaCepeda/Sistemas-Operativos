GCC = gcc
FLAGS = -c

PROGRAMAS: lab_Principal

lab_Principal:
	$(GCC) $(FLAGS) Libreria_lab.c
	$(GCC) $@.c Libreria_lab.o -o $@
	$(RM) *.o

clear: 
	%(RM) *.o lab_Principal