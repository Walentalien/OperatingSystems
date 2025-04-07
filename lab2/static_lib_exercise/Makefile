CC = gcc



add.o: add.c
	$(CC) -c add.c

sub.o: sub.c
	$(CC) -c sub.c

lib_calc.a: add.o sub.o
	ar rcs lib_calc.a add.o sub.o

calc.o: calc.c
	gcc -c calc.c -o calc.o

calc_static: calc.o lib_calc.a
	gcc -o calc calc.o -L. lib_calc.a


clean:
	rm -f *.o *.a *.out app
