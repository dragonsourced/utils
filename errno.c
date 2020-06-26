#include <stdio.h>
#include <string.h>

int
main(int argc, const char **argv)
{
	if (argc > 1) {
		for (int i = 1; i < argc; ++i) {
			int no;
			sscanf(argv[i], "%d", &no);
			printf("%d\t%s\r\n", no, strerror(no));
		}
	} else {
		printf("Usage: %s ERRNO [ERRNO2...]\r\n", argv[0]);
	}

	return 0;
}
