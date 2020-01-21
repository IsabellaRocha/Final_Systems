ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: control.o main.o return_car.o parse.o headers.h
	$(CC) -o control control.o
	$(CC) -o program main.o parse.o
	$(CC) -o return_car return_car.o

control.o: control.c
	$(CC) -c control.c

main.o: main.c
	$(CC) -c main.c

parse.o: parse.c
	$(CC) -c parse.c

return_car.o: return_car.c
	$(CC) -c return_car.c

run:
	./program

clean:
	rm *.o
	rm *.txt
