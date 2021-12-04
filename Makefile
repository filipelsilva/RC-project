.PHONY: all ds user clean entr

CC=g++
CFLAGS=

all: ds user

ds: server/ds.cpp
	$(CC) $(CFLAGS) server/ds.cpp -o bin/DS

user: client/user.cpp
	$(CC) $(CFLAGS) client/user.cpp -o bin/user

clean: bin/DS bin/user
	rm bin/DS bin/user

entr:
	find . -name "*.cpp" !-name "*.c" ! -name "*.h" | entr make
