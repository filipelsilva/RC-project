.PHONY: all ds user clean entr

CC=g++
CFLAGS=-Wall -Wextra

all: ds user

ds: server/ds.cpp
	$(CC) $(CFLAGS) server/ds.cpp -o bin/DS

user: client/user.cpp
	$(CC) $(CFLAGS) client/user.cpp -o bin/user

# TO REMOVE LATER {{{
main: server/main.cpp
	$(CC) $(CFLAGS) server/main.cpp -o bin/main

tcp: client/TCPClient.cpp
	$(CC) $(CFLAGS) client/TCPClient.cpp -o bin/tcp

udp: client/UDPClient.cpp
	$(CC) $(CFLAGS) client/UDPClient.cpp -o bin/udp

servers: server/TCPServer.cpp server/UDPServer.cpp server/clean.cpp
	$(CC) $(CFLAGS) server/UDPServer.cpp -o bin/udp_server
	$(CC) $(CFLAGS) server/TCPServer.cpp -o bin/tcp_server
	$(CC) $(CFLAGS) server/clean.cpp -o bin/clean
# }}}

clean:
	rm bin/*

format:
	clang-format -style="{IndentWidth: 4, TabWidth: 4, UseTab: AlignWithSpaces}" -i **/*.cpp

zip: # Uses git, not very portable in zip
	git archive -o projeto_rc.zip HEAD

entr: # Uses entr, not very portable
	find . -name "*.cpp" ! -name "*.c" ! -name "*.h" | entr make
