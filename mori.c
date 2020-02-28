#include <string.h>
#include <stdio.h>
#include <time.h>

#define MINUTE 60
#define HOUR   (60 * MINUTE)
#define DAY    (24 * HOUR)
#define WEEK   (7 * DAY)
#define YEAR   (365 * DAY)

#define LIFESPAN 80

int main(int argc, char *argv[])
{
	time_t current;

	struct tm birth = {
		.tm_mon = 2,
		.tm_year = 104,
		.tm_mday = 2
	};

	struct tm death = {
		.tm_mon = 2,
		.tm_year = 104 + LIFESPAN,
		.tm_mday = 2
	};

	time_t death_time = mktime(&death);
	time_t birth_time = mktime(&birth);

	time(&current);
	time_t remaining = death_time - current;

	int weeks, days, hours, minutes, seconds;
	long double used, total, left;

	total = death_time - birth_time;
	left  = remaining;
	used  = total - left;

	remaining -= (weeks    = (remaining / WEEK))   * WEEK;
	remaining -= (days     = (remaining / DAY))    * DAY;
	remaining -= (hours    = (remaining / HOUR))   * HOUR;
	remaining -= (minutes  = (remaining / MINUTE)) * MINUTE;
	seconds = remaining;

	if (argc == 1)
		printf("%d:%d:%d:%d:%d\n", weeks, days, hours, minutes, seconds);
	else if (argc == 2 && !strcmp(argv[1], "%left"))
		printf("%Lf\n", left / (total / 100));
	else if (argc == 2 && !strcmp(argv[1], "%used"))
		printf("%Lf\n", used / (total / 100));
	else
		fprintf(stderr, "Usage: %s [%%left/%%used]\n", argv[0]);
}
