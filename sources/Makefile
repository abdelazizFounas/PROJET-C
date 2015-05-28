CC = gcc
SRCS = main.c structure.c compresseur.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -I ../includes
EXEC = main

all: $(EXEC) clean

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) -Werror -Wall -Wextra --std=c99

clean:
	rm -rf *.o

mrproper: clean
	rm -rf