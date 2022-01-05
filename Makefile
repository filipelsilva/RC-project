.PHONY: all ds user main clean format zip entr

CC=g++
CFLAGS=-Wall -Wextra

all: ds user

ds: server/ds.cpp
	$(CC) $(CFLAGS) server/ds.cpp -o bin/DS

user: client/user.cpp
	$(CC) $(CFLAGS) client/user.cpp -o bin/user

main: server/main.cpp
	$(CC) $(CFLAGS) server/main.cpp -o bin/main

clean:
	rm bin/*

format:
	clang-format -style="{IndentWidth: 4, TabWidth: 4, UseTab: AlignWithSpaces}" -i **/*.cpp **/*.hpp

zip:
	git archive -o proj_13.zip HEAD
