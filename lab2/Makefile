CC = gcc

# compile w/o linking
collatz.o: collatz.c
	$(CC) -c collatz.c
# create archive (static library)
lib_collatz.a: collatz.o
	ar rcs lib_collatz.a collatz.o
# compile test program but w/o linking
test_collatz.o: test_collatz.c
	gcc -c test_collatz.c -o test_collatz.o
# creates an executable with linked static library
test_collatz_static: test_collatz.o lib_collatz.a
	gcc -o test_collatz_static test_collatz.o -L. lib_collatz.a

# build shared library
lib_collatz.so: collatz.o
	$(CC) -shared -o lib_collatz.so collatz.o

# build executable linked with shared library
test_collatz_shared: test_collatz.o lib_collatz.so
	$(CC) -o test_collatz_shared test_collatz.o -L. -lcollatz -Wl,-rpath=.

# Tf why it wasn't working???
run_test_collatz_shared:
	export LD_LIBRARY_PATH=. && ./test_collatz_shared



#
# FOR DYNAMICALLY LOADED LIBRARY
#
CFLAGS += -DDYNAMIC_LOADING
test_collatz_dynamic: test_collatz.o lib_collatz.so
	$(CC) -o test_collatz_dynamic test_collatz.o -L. lib_collatz.so -ldl -Wl,-rpath=.

clean:
	rm -f *.o *.a *.out app
