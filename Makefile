# Variables                                                                                                                                                                                   
CC=gcc
CFLAGS=-std=c11 -W -Wall -pedantic
EXEC=networkTest

RED     =       "\e[1;31m"
WHITE   =       "\e[0m"

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
        @`which echo` -e $(RED)"[RM] "$(OBJ)$(WHITE)
        @rm -Rv *.o

fclean: clean
        @`which echo` -e $(RED)"[RM] "$(EXEC)$(WHITE)
        @rm -Rv $(EXEC)
