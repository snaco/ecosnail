CC = g++
CFLAGS = -std=c++11 -I../../include/

all: hello

hello: hello.cpp
	$(CC) $(CFLAGS) -o hello hello.cpp

.PHONY: clean
clean:
	rm -f hello.o hello
