CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: main

main: main.o neural_network.o math_nn.o helpers.o
	$(CC) $(CFLAGS) -o main main.o neural_network.o math_nn.o helpers.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

neural_network.o: neural_network.c
	$(CC) $(CFLAGS) -c neural_network.c

math_nn.o: math_nn.c
	$(CC) $(CFLAGS) -c math_nn.c

helpers.o: helpers.c
	$(CC) $(CFLAGS) -c helpers.c

clean:
	rm -f *.o main
#ENDMAKE