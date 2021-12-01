.PHONY: all ds user clean

all: ds user

ds: server/ds.c
	gcc server/ds.c -o bin/DS

user: client/user.c
	gcc client/user.c -o bin/user

clean: bin/DS bin/user
	rm bin/DS bin/user
