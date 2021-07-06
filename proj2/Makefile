CC = gcc
TARGET = testlib

$(TARGET): main.o list.o hash.o bitmap.o hex_dump.o
		   $(CC) -o $(TARGET) main.o list.o hash.o bitmap.o hex_dump.o -lm

main.o: main.c bitmap.h hash.h list.h
		$(CC) -c main.c

list.o: list.c list.h
		$(CC) -c list.c

hash.o: hash.c hash.h
		$(CC) -c hash.c -lm

hex_dump.o: hex_dump.c hex_dump.h
		$(CC) -c hex_dump.c 

bitmap.o: bitmap.c bitmap.h limits.h round.h hex_dump.h
		$(CC) -c -w bitmap.c

clean:
		rm *.o $(TARGET)
