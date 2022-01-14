# Computer Networks Project, 2021/2022

This is a simple client/server application, implementing a prototype of a
centralized messaging service. In order to test it, simply run `make` in this
folder. The executables and necessary folders will be placed there.

As stated before, this project is split in two parts:

- User, the client, which runs with the following (optional) parameters:

./user [-n DSIP] [-p DSport]
	-n DSIP		IP address of the machine where the DS is running
	-p PORT		Port where the DS server accepts requests

- DS, the server, which runs with the following (optional) parameters:

./DS [-p DSport] [-v]
	-p PORT		Port where the DS server accepts requests
	-v			Verbose mode: outputs description of the received requests

- Note: DSIP defaults to "localhost" and DSPORT defaults to "58013". Verbose
  mode is disabled by default.

There are a number of commands that can be tested, involving user registration,
user acess, group membership and message handling. Below is the list of
commands:

- reg UID pass
- unregister UID pass / unr UID pass
- login UID pass
- logout
- showuid / su
- exit
- groups / gl
- subscribe GID GName / s GID GName
- unsubscribe GID / u GID
- my_groups / mgl
- select GID / sag GID
- showgid / sg
- ulist / ul
- post "text" [Fname]
- retrieve MID / r MID
