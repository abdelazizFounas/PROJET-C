CC = gcc
SRCS = main.c structuren.c compresseur.c decompresseur.c
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