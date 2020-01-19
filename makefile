all: initialize.o user.o rent.o
	gcc -o initialize initialize.o
	gcc -o program user.o
	gcc -o rent rent.o

initialize.o: initialize.c
	gcc -c initialize.c

user.o: user.c
	gcc -c user.c

rent.o: rent.c
	gcc -c rent.c

run:
	./program

clean:
	rm *.o
	rm *.txt
