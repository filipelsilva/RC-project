.PHONY: all DS user main clean clean-data format todo zip

CC=g++
CFLAGS=-Wall -Wextra

all: DS user

DS: server/ds.cpp
	$(CC) $(CFLAGS) $< -o bin/$@

user: client/user.cpp
	$(CC) $(CFLAGS) $< -o bin/$@

clean:
	find bin/ -maxdepth 1 -type f ! -iname ".*" -delete

clean_data:
	rm -r bin/USERS/* bin/GROUPS/*

format:
	clang-format -style="{IndentWidth: 4, TabWidth: 4, UseTab: AlignWithSpaces}" -i **/*.cpp **/*.hpp

todo:
	grep --line-number --colour=always -r -i --include=\*.cpp --include=\*.hpp "todo"

zip:
	git archive -o proj_13.zip HEAD
