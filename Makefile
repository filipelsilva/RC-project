.PHONY: all ds user main clean format zip

CC=g++
CFLAGS=-Wall -Wextra

all: ds user

ds: server/ds.cpp
	$(CC) $(CFLAGS) $< -o bin/$@ && mv bin/ds bin/DS

user: client/user.cpp
	$(CC) $(CFLAGS) $< -o bin/$@

main: server/main.cpp
	$(CC) $(CFLAGS) $< -o bin/$@

clean:
	rm bin/*

format:
	clang-format -style="{IndentWidth: 4, TabWidth: 4, UseTab: AlignWithSpaces}" -i **/*.cpp **/*.hpp

zip:
	git archive -o proj_13.zip HEAD
