#include <ctype.h>
#include <stdio.h>

int
main(void)
{
	char buf[1024];
	char *s;

	while (fgets(buf, 1024, stdin))
		for (s = buf; s && *s; ++s)
			if (!isspace(*s) || (isalnum(*(s-1)) && isalnum(*(s+1))))
				putchar(*s);

	return 0;
}
