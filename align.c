#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define WIDTH 64

void
margin(size_t size)
{
	for (size_t i = 0; i < size; ++i)
		putchar(' ');
}

void
align_line(const char *line, const char a)
{
	char buf[1024];
	size_t len = 0;

	memset(buf, 0, 1024);
	while (isspace(*line)) ++line;
	while (*line != '\n') buf[len++] = *line++;
	while (isspace(buf[--len])) buf[len] = '\0';
	len++;

	switch (a) {
	case 'l':
		puts(buf);
		break;
	case 'c':
		margin(WIDTH/2 - len/2);
		puts(buf);
		break;
	case 'r':
		margin(WIDTH - len);
		puts(buf);
		break;
	}
}

int
invalid_alignment(const char *a)
{
	return strlen(a) > 1 || (*a != 'l' && *a != 'c' && *a != 'r');
}

int
main(int argc, const char **argv)
{
	char buf[1024];

	if (argc != 2 || invalid_alignment(argv[1])) {
		fprintf(stderr, "Usage: %s [lcr]\n", argv[0]);
		return 1;
	}

	while (fgets(buf, 1024, stdin))
		align_line(buf, *argv[1]);

	return 0;
}
