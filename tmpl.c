/* tmpl.c
 *
 * A dead simple templating system.
 *
 * It can insert envirionment variables, execute shell commands,
 * and do absolutely nothing else.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <err.h>

int
parse(const char *str)
{
	const char *v;
	char p[1024];
	memset(p, 0, 1024);

	while (isspace(*str))
		fputc(*str++, stdout);

	for (int i = 0; i < 1024 && isprint(str[i]); ++i)
		p[i] = str[i];

	switch (*str) {
		case '!':
			system(str + 1);
			break;
		case '$':
			v = getenv(p + 1);
			if (v)
				printf("%s\n", v);
			else
				printf("$%s\n", p + 1);
			break;
		default:
			printf("%s", str);
	}
}

int
main(int argc, const char **argv)
{
	for (int i = 1; i < argc; ++i) {
		FILE *f = fopen(argv[i], "r");
		if (f) {
			char buf[1024];

			while (fgets(buf, 1023, f))
				parse(buf);

			fclose(f);
		}
	}

	return 0;
}
