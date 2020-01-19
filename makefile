all: user.o rent.o initialize.o
	gcc -o program user.o
	gcc -o rent rent.o
	gcc -o initialize initialize.o

user.o: user.c
	gcc -c user.c

rent.o: rent.c
	gcc -c rent.c

initialize.o: initialize.c
	gcc -c initialize.c

run:
	./program

clean:
	rm *.o
	rm *.txt
