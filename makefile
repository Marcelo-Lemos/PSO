################################## VARIABLES ###################################

# Compiler and flags
CC = gcc
MPICC = mpicc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2

#################################### RULES #####################################

# Rules for compiling
all: sequential-pso.exe parallel-pso.exe

sequential-pso.exe: src/sequential-pso.c src/particle.c src/particle.h
	$(CC) $(CFLAGS) src/sequential-pso.c src/particle.c -o $@

parallel-pso.exe: src/parallel-pso.c src/particle.c src/particle.h
	$(MPICC) $(CFLAGS) src/parallel-pso.c src/particle.c -o $@

# Rule for cleaning the directory
clean:
	rm -rf *.exe $(OBJDIR)

.PHONY: all clean
