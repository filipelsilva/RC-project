#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DSPORT_DEFAULT 58000 // TODO: update default number

int main(int argc, char** argv) {
	int flag;
	char* DSip = "localhost";
	int tmp, DSport = DSPORT_DEFAULT;

	while ((flag = getopt(argc, argv, "n:p:")) != -1) {
		switch (flag) {
			case 'n':
				DSip = optarg;
				break;

			case 'p':
				if ((tmp = strtol(optarg, NULL, 10)) == 0) {
					exit(1);
				}
				DSport = tmp;
				break;

			default:
				break;
		}
	}

	printf("DSip: %s\nDSport: %d\n", DSip, DSport);

	return 0;
}
