################################## VARIABLES ###################################

# Compiler and flags
CC = gcc
MPICC = mpicc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2

#################################### RULES #####################################

# Rules for compiling
all: sequential-pso.exe

sequential-pso.exe: src/sequential-pso.c
	$(CC) $(CFLAGS) src/sequential-pso.c -o $@

# Rule for cleaning the directory
clean:
	rm -rf *.exe $(OBJDIR)

.PHONY: all clean
