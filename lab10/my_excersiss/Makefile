CC = gcc
CFLAGS = -Wall -pthread
TARGET = cond_var

all: $(TARGET)

$(TARGET): cond_var.o
	$(CC) $(CFLAGS) -o $(TARGET) cond_var.o

cond_var.o: cond_var.c
	$(CC) $(CFLAGS) -c cond_var.c

.PHONY: clean

clean:
	rm -f *.o *.out *.exe core core.* *.stackdump a.out
	find . -type f -perm -111 -exec rm -f {} +
