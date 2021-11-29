.PHONY: all ds user clean

all: server client

ds: server/ds.c
	gcc server/ds.c -o DS

user: client/user.c
	gcc client/user.c -o user

clean:
	rm DS user
