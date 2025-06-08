CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = main
OBJS = main.o palindrome.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

main.o: main.c palindrome.h
	$(CC) $(CFLAGS) -c main.c

palindrome.o: palindrome.c palindrome.h
	$(CC) $(CFLAGS) -c palindrome.c

clean:
	rm -f $(OBJS) $(TARGET)

start:
	./main

.PHONY: clean
