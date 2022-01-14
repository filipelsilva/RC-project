.PHONY: all DS user debug clean clean_data todo zip

CC=g++
CFLAGS=-Wall -Wextra
CFLAGS_DEBUG=-g

all: DS user

DS: server/ds.cpp
	$(CC) $(CFLAGS) $< -o $@

user: client/user.cpp
	$(CC) $(CFLAGS) $< -o $@

debug: server/ds.cpp client/user.cpp
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) server/ds.cpp -o DS
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) client/user.cpp -o user

clean:
	rm ./DS ./user

clean_data:
	rm -r USERS/* GROUPS/*

todo:
	grep --line-number --colour=always -r -i --include=\*.cpp --include=\*.hpp "todo"

zip:
	git archive -o proj_13.zip HEAD
