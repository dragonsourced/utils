/* tmpl.c
 *
 * A barebones templating system, which allows files to include
 * other files, perform commands, and pretty much nothing else.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

void
include_file(const char *f)
{
	char fb[strlen(f)];
	char *s = fb;

	memset(fb, 0, strlen(f));

	while (isprint(*f))
		*s++ = *f++;

	puts(fb);
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
	if (*line == '.')
		command(line);
	else if (*line == '!')
		system(line + 1);
	else
		printf(line);
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
