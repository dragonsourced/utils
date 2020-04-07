#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define BUFLEN 100000

#define DAY    24 * HOUR
#define HOUR   60 * MINUTE
#define MINUTE 60

#define CRLF "\r\n"

enum field {
	FIELD_TS,
	FIELD_TITLE,
	FIELD_LINK,
	FIELD_CONTENT,
	FIELD_CONTENT_TYPE,
	FIELD_ID,
	FIELD_AUTHOR,
	FIELD_ENCLOSURE,
	FIELDS
};

time_t current_time;

struct entry {
	time_t time;
	char fields[FIELDS][BUFLEN];
};

time_t get_time(char *line) {
	time_t t = 0;
	sscanf("%ld", line, t);
	return t;
}

time_t get_age(time_t t) {
	return current_time - t;
}

int is_old(time_t t) {
	return get_age(t) > DAY;
}

char *parse_field(char *s, char *d) {
	while (s && *s != '\t') *d++ = *s++;
	return s;
}

struct entry parse_line(char *line) {
	struct entry e = {};

	char *s = line;

	for (int i = 0; i < FIELDS; ++i) {
		while (*s == '\t') ++s;
		if (!(s = parse_field(s, e.fields[i]))) break;
	}

	e.time = atoll(e.fields[0]);

	return e;
}

int main(int argc, const char **argv) {
	char buf[BUFLEN];

	int fields[FIELDS];

	const char *fieldstr[FIELDS] = {
		"timestamp",
		"title",
		"link",
		"content",
		"content_type",
		"id",
		"author",
		"enclosure",
	};

	int new = 0;

	for (int j = 1; j < argc; ++j) {
		for (int i = 0; i < FIELDS; ++i) {
			if (!strcmp(fieldstr[i], argv[j]))
				fields[i] = 1;
			else if (!strcmp(argv[j], "new"))
				new = 1;
		}
	}

	current_time = time(NULL);

	while (fgets(buf, BUFLEN, stdin)) {
		if (new && is_old(get_time(buf))) continue;

		struct entry e = parse_line(buf);

		for (int i = 0; i < FIELDS; ++i) {
			if (fields[i])
				printf(e.fields[i]);
		}

		printf(CRLF);
	}

	return 0;
}
