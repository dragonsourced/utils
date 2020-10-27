#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int
main(int argc, const char **argv)
{
	char buf[1024];
	int cps = 20; /* characters per second */

	if (argc == 2) {
		sscanf(argv[1], "%d", &cps);
	}

	useconds_t t = 1000000 / cps;

	system("clear");

	while (fgets(buf, 1024, stdin)) {
		for (int i = 0; i < strlen(buf); ++i) {
			putc(buf[i], stdout);
			fflush(stdout);
			usleep(t);
		}
	}

	return 0;
}
