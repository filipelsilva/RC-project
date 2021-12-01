#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DSPORT_DEFAULT 58013

int main(int argc, char** argv) {
	char flag;
	char* DSip = "localhost";
	int tmp, DSport = DSPORT_DEFAULT, verbose = 0;

	while ((flag = getopt(argc, argv, "p:v")) != -1) {
		switch (flag) {
			case 'p':
				if ((tmp = strtol(optarg, NULL, 10)) == 0) {
					exit(1);
				}
				DSport = tmp;
				break;

			case 'v':
				verbose = 1;
				break;

			default:
				break;
		}
	}

	printf("DSport: %d\nVerbose mode: %d\n", DSport, verbose);

	return 0;
}
