ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: control.o main.o user.o rent.o return_car.o parse.o headers.h
	$(CC) -o control control.o parse.o
	$(CC) -o program main.o user.o parse.o rent.o return_car.o

main.o: main.c
	$(CC) -c main.c

control.o: control.c
	$(CC) -c control.c

parse.o: parse.c
	$(CC) -c parse.c

return_car.o: return_car.c
	$(CC) -c return_car.c

run:
	./program

clean:
	rm *.o
	rm *.txt
