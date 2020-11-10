#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

int
main(int argc, const char **argv)
{
	struct timeval t, _t;
	char buf[1024];
	int cps = 20; /* characters per second */

	if (argc == 2) {
		sscanf(argv[1], "%d", &cps);
	}

	t.tv_usec = 1000000 / cps;

	system("clear");

	while (fgets(buf, 1024, stdin)) {
		for (int i = 0; i < strlen(buf); ++i) {
			putc(buf[i], stdout);
			fflush(stdout);
			_t = t;
			select(0, NULL, NULL, NULL, &_t);
		}
	}

	return 0;
}
