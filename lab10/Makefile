CC = gcc
CFLAGS = -Wall -pthread
TARGET = hospital_sim

all: $(TARGET)

$(TARGET): main.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

.PHONY: clean

clean:
	rm -f *.o *.out *.exe core core.* *.stackdump a.out
	find . -type f -perm -111 -exec rm -f {} +
