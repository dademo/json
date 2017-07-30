# Variables
CC=gcc
CFLAGS=-std=c11 -W -Wall -pedantic
EXEC=networkTest

SRC=fct_utiles.c chainedlist.c htmlRequest.c json_chainedlist.h json.c myCRC.c main.c 
OBJ=$(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

myTest: chainedlistTest.o chainedlist.o
	$(CC) -o myTest $^ $(CFLAGS)

#%.o: %.c %.h
%.o: %.c
	$(CC) -c $^ $(CFLAGS)

# Exécution systématique
.PHONY: clean

# Nettoyage
clean:
	@rm -Rv *.o
	@rm -Rv */*.o
