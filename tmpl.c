/* tmpl.c
 *
 * A dead simple templating system.
 *
 * It can insert envirionment variables, execute shell commands,
 * and do absolutely nothing else.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <err.h>

char *
strip(char *buf)
{
	char *s = buf;
	while (*++s)
		if (*s == '\n')
			*s = '\0';
	return buf;
}

void
var(char *s)
{
	s = strip(++s);
	printf("%s\n", getenv(s));
}

void
sh(char *s)
{
	system(++s);
}

void
file(FILE *f)
{
	char buf[1024];
	while (fgets(buf, 1023, f))
		if (*buf == '$')
			var(buf);
		else if (*buf == '!')
			sh(buf);
		else
			printf("%s", buf);
}

int
main(int argc, const char **argv)
{
	int i;

	if (argc < 2) {
		file(stdin);
	} else {
		for (i = 1; i < argc; ++i) {
			FILE *f = fopen(argv[i], "r");
			if (f)
				file(f);
			else
				err(1, "%s", argv[i]);
		}
	}

	return 0;
}
