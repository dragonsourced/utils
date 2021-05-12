#define _POSIX_C_SOURCE 1

#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct termios orig_attr;

#define STDIN_FD fileno(stdin)
#define STDOUT_FD fileno(stdout)
#define WRAP_LEN 72
#define TAB_STOP 8

void
setup(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);

	tcgetattr(STDIN_FD, &orig_attr);

	struct termios t = orig_attr;
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 2;
	tcsetattr(STDIN_FD, TCSANOW, &t);
}

char *text;
size_t textl = 0;
size_t texts = 0;

void
cleanup(void)
{
	free(text);
	tcsetattr(STDIN_FD, TCSANOW, &orig_attr);
}

static int should_echo = 1;

void
addch(const char ch)
{
	switch (ch) {
	case '\b':
		if (should_echo) {
			fputc('\b', stderr);
			fputc(' ', stderr);
			fputc('\b', stderr);
		}
		textl--;
		break;
	default:
		if (should_echo) {
			fputc(ch, stderr);
		}
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
		curs->x--;
	}
	curs->x = (curs->x < 0? 0 : curs->x);
	if (curs->x > 0) {
		while (curs->x > 0 && isspace(text[textl-1])) {
			addch('\b');
			curs->x--;
		}
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

	case '\t':
		addch('\t');
		do {
			curs->x++;
		} while (curs->x % TAB_STOP != 0);
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

void
wrap_file(FILE *f)
{
	struct cursor curs = { 0 };
	char ch;

	should_echo = isatty(fileno(f)) || isatty(fileno(stdout));

	while ((ch = fgetc(f))) {
		if (ch == EOF || ch == 4) { /* End Of Transmission, ^D */
			break;
		}
		putch(ch, &curs);
	}
}

int
main(int argc, const char **argv)
{
	setup();

	if (argc > 1) for (int i = 1; i < argc; ++i) {
		FILE *f = fopen(argv[i], "r");
		wrap_file(f);
		fclose(f);
	} else {
		wrap_file(stdin);
	}

	if (!isatty(STDOUT_FD)) {
		printf("%s", text);
	}

	cleanup();

	return 0;
}
