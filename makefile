all: initialize.o user.o rent.o ret.o headers.h
	gcc -o initialize initialize.o
	gcc -o program user.o
	gcc -o rent rent.o
	gcc -o ret ret.o

initialize.o: initialize.c
	gcc -c initialize.c

user.o: user.c
	gcc -c user.c

rent.o: rent.c
	gcc -c rent.c

ret.o: ret.c
	gcc -c ret.c

run:
	./program

clean:
	rm *.o
	rm *.txt
