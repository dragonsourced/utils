/* tmpl.c
 *
 * A barebones templating system.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

void
printable(const char *src, char *dst, size_t len)
{
	for (int i = 0; i < len && isprint(src[i]); ++i)
		dst[i] = src[i];
}

void
include_file(const char *f)
{
	puts(f);
}

void
command(const char *cmd)
{
	switch (*(++cmd)) {
		case 'I':
			include_file(cmd + 2);
			break;
		default:
			fprintf(stderr, "UNKNOWN COMMAND: '%s'\r\n", cmd);
	}
}

void
parse(const char *line)
{
	char p[1024];
	printable(line, p, 1024);

	switch (*line) {
		case '.':
			command(p);
			break;
		case '$':
			printf("%s\r\n", getenv(p + 1));
			break;
		case '!':
			system(p + 1);
			break;
		default:
			printf(line);
	}
}

void
parse_file(FILE *f)
{
	char buf[1025];
	while (fgets(buf, 1024, f))
		parse(buf);
}

int
main(int argc, const char **argv)
{
	FILE *f;
	int i;

	if (argc > 1) {
		for (i = 1; i < argc; ++i) {
			f = fopen(argv[i], "r");
			if (f) {
				parse_file(f);
				fclose(f);
			} else {
				fprintf(stderr,
				    "%s: %s: No such file or directory.\r\n",
				    argv[0],
				    argv[i]);
			}
		}
	}
}
