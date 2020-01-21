ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: control.o user.o rent.o return_car.o parse.o headers.h
	$(CC) -o control control.o
	$(CC) -o program user.o parse.o
	$(CC) -o rent rent.o
	$(CC) -o return_car return_car.o

control.o: control.c
	$(CC) -c control.c

user.o: user.c
	$(CC) -c user.c

parse.o: parse.c
	$(CC) -c parse.c

rent.o: rent.c
	$(CC) -c rent.c

return_car.o: return_car.c
	$(CC) -c return_car.c

run:
	./program

clean:
	rm *.o
	rm *.txt
