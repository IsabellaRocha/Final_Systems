all: control.o user.o rent.o return_car.o parse.o headers.h
	gcc -o control control.o
	gcc -o program user.o parse.o
	gcc -o rent rent.o
	gcc -o return_car return_car.o

control.o: control.c
	gcc -c control.c

user.o: user.c
	gcc -c user.c

parse.o: parse.c
	gcc -c parse.c
	
rent.o: rent.c
	gcc -c rent.c

return_car.o: return_car.c
	gcc -c return_car.c

run:
	./program

clean:
	rm *.o
	rm *.txt
