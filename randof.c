#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
	time_t current;
	int i;

	if (argc < 2) {
		printf("Usage: %s [OPT]...", argv[0]);
		puts("");
		puts("Display a random OPT.");
		return 1;
	}

	time(&current);

	current *= 10000;
	current += clock();
	srand(current);

	i = (rand() % (argc - 1)) + 1;
	printf("%s\n", argv[i]);

	return 0;
}
