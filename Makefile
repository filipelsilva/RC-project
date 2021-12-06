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

zip: # Uses git, not very portable in zip
	git archive -o proj_58013.zip HEAD

entr: # Uses entr, not very portable
	find . -name "*.cpp" !-name "*.c" ! -name "*.h" | entr make
