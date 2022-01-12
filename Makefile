.PHONY: all DS user main clean format zip

CC=g++
CFLAGS=-Wall -Wextra
CFLAGS_DEBUG=-g

all: DS user

DS: server/ds.cpp
	$(CC) $(CFLAGS) $< -o bin/$@

user: client/user.cpp
	$(CC) $(CFLAGS) $< -o bin/$@

debug: server/ds.cpp client/user.cpp
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) server/ds.cpp -o bin/DS
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) client/user.cpp -o bin/user

clean:
	find bin/ -maxdepth 1 -type f ! -iname ".*" -delete

format:
	clang-format -style="{IndentWidth: 4, TabWidth: 4, UseTab: AlignWithSpaces}" -i **/*.cpp **/*.hpp

todo:
	grep --line-number --colour=always -r -i --include=\*.cpp --include=\*.hpp "todo"

zip:
	git archive -o proj_13.zip HEAD
