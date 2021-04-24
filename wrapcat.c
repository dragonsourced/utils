#define _POSIX_C_SOURCE 1

#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct termios orig_attr;

#define STDOUT_FD 0
#define WRAP_LEN 72

void
setup(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);

	tcgetattr(STDOUT_FD, &orig_attr);

	struct termios t = orig_attr;
	t.c_lflag &= ~ICANON & ~ECHO;
	t.c_lflag |= ISIG | VEOF;
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 2;
	tcsetattr(STDOUT_FD, TCSANOW, &t);
}

char *text;
size_t textl = 0;
size_t texts = 0;

void
cleanup(void)
{
	free(text);
	tcsetattr(STDOUT_FD, TCSANOW, &orig_attr);
}

void
addch(const char ch)
{
	switch (ch) {
	case '\b':
		fputc('\b', stderr);
		fputc(' ', stderr);
		fputc('\b', stderr);
		textl--;
		break;
	default:
		fputc(ch, stderr);
		textl++;
	}

	if (texts == 0) {
		texts = textl;
		text = malloc(texts);
	} else while (texts < textl) {
		texts *= 2;
		text = realloc(text, texts);
	}

	switch (ch) {
	case '\b':
		text[textl] = 0;
		break;

	default:
		text[textl-1] = ch;
	}

}

void
adds(const char *s)
{
	while (*s) {
		addch(*s++);
	}
}

struct cursor {
	char wrd[1024];
	unsigned int wrd_i;
	unsigned int x;
};

void
erase_word(struct cursor *curs)
{
	int max = curs->wrd_i;

	for (int i = 0; i < max; ++i) {
		addch('\b');
	}
}

void
putch(char ch, struct cursor *curs)
{
	switch (ch) {
	case '\n':
		addch('\n');
		curs->x = 0;
		break;

	case '\b':
	case 0x7F: /* DEL */
		if (curs->x > 0) {
			addch('\b');
			curs->x--;
		}
		break;

	default:
		if (!isprint(ch)) {
			break;
		}

		if (isspace(ch)) {
			memset(curs->wrd, 0, 1024);
			curs->wrd_i = 0;
		} else {
			curs->wrd[curs->wrd_i++] = ch;
		}

		addch(ch);
		curs->x++;
		if (curs->x > WRAP_LEN) {
			erase_word(curs);
			addch('\n');
			adds(curs->wrd);
			memset(curs->wrd, 0, 1024);
			curs->x = curs->wrd_i;
			curs->wrd_i = 0;
		}
	}
}

int
main(int argc, const char **argv)
{
	FILE *f;
	setup();

	if (argc > 1) {
		f = fopen(argv[1], "r");
	} else {
		f = stdin;
	}

	struct cursor curs = { 0 };
	char ch;

	while ((ch = fgetc(f))) {
		if (ch == EOF || ch == 4) { /* End Of Transmission, ^D */
			break;
		}
		putch(ch, &curs);
	}

	if (!isatty(fileno(stdout))) {
		printf("%s", text);
	}

	cleanup();

	return 0;
}
