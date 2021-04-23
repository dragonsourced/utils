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
	t.c_lflag &= ISIG;
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 2;
	tcsetattr(STDOUT_FD, TCSANOW, &t);
}

void
cleanup(void)
{
	tcsetattr(STDOUT_FD, TCSANOW, &orig_attr);
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
		putchar('\b');
	}

	for (int i = 0; i < max; ++i) {
		putchar(' ');
	}
}

void
putch(char ch, struct cursor *curs)
{
	switch (ch) {
	case '\n':
		printf("\n");
		curs->x = 0;
		break;

	default:
		if (isspace(ch)) {
			memset(curs->wrd, 0, 1024);
			curs->wrd_i = 0;
		} else {
			curs->wrd[curs->wrd_i++] = ch;
		}

		putchar(ch);
		curs->x++;
		if (curs->x > WRAP_LEN) {
			erase_word(curs);
			printf("\n%s", curs->wrd);
			memset(curs->wrd, 0, 1024);
			curs->x = curs->wrd_i;
			curs->wrd_i = 0;
		}
	}
}

int
main(void)
{
	setup();

	struct cursor curs = { 0 };
	char ch;

	while ((ch = getchar())) {
		putch(ch, &curs);
	}

	cleanup();

	return 0;
}
