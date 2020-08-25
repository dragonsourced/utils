#include <stdio.h>
#include <string.h>

void
cmd(const char *cmd)
{
	char buf[1024];
	char *p;

	strncpy(buf, cmd, 1023);
	p = buf + strlen(buf) - 1;
	while (*p == '\n') *p-- = '\0';

	switch (*cmd) {
	case 'T':
		printf("<title>%s</title>\n", buf + 2);
		break;
	case 'H':
		printf("<h%c>%s</h%c>\n", *(buf + 1), buf + 3, *(buf + 1));
		break;
	case 'P':
		puts("<p>");
		break;
	case 'I':
		printf("<i>%s</i>\n", buf + 2);
		break;
	case 'B':
		printf("<b>%s</b>\n", buf + 2);
		break;
	case 'A':
		p = buf + 2;
		printf("<a href=\"");
		while (*p && *p != ' ') fputc(*p++, stdout);
		printf("\">");
		while (*p == ' ') ++p;
		while (*p) fputc(*p++, stdout);
		printf("</a>\n");
		break;
	default:
		fprintf(stderr, "UNKNOWN COMMAND: .%s\n", buf);
	}
}

void
parse(const char *ln)
{
	if (*ln == '.')
		cmd(ln + 1);
	else
		printf("%s", ln);
}

int
main(void)
{
	char buf[1024];

	while (fgets(buf, 1024, stdin))
		parse(buf);

	return 0;
}
