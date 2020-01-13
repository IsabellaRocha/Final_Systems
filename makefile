all: user.o
	gcc -o program user.o

user.o: user.c
	gcc -c user.c

run:
	./program

clean:
	rm *.o
	rm *.exe
	rm *.exe.stackdump
