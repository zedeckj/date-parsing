#include "date.h"
#include <stdbool.h>

int tests = 0;

void test_err(char *date_str, char *expect) {
	date_t date;
	const char *err = parse_date(&date, date_str, '.');
	if (!err) {
		fprintf(stderr, "No error given for %s\n", date_str);
		exit(1);
	}
	if (strcmp(err, expect)) {
		fprintf(stderr, "Error '%s' does not match expected '%s'\n", err, expect);
		exit(1);
	}
	tests += 1;
}

void test_eq(char * date_str, int year, int month, int day) {
	date_t date;
	const char *err = parse_date(&date, date_str, '.');
	if (err) {
		fprintf(stderr, "Invalid date %s: %s\n", date_str, err);
		exit(1);
	}
	if (date.year != year || date.month != month || date.day != day) {
		fprintf(stderr, "Date %d.%d.%d does not match expected %d.%d.%d\n", 
				date.year,
				date.month,
				date.day,
				year,
				month,
				day);
		exit(1);
	}
	tests += 1;
	return;
}




void test_basic(){
	test_eq("2020.12.01", 2020, 12, 1);
	test_eq("0001.04.12", 1, 4, 12);
	test_eq("2020.06.23", 2020, 6, 23);
	test_eq("2000.05.04", 2000, 5, 4);
	test_eq("1971.04.16", 1971, 4, 16);
	test_eq("0999.02.20", 999, 2, 20);
	test_eq("4949.07.30", 4949, 7, 30);
	test_eq("1999.02.03", 1999, 2, 3);
}

void test_limits() {
	test_eq("2000.01.31", 2000, 1, 31);
	test_eq("2000.03.31", 2000, 3, 31);
	test_eq("2000.04.30", 2000, 4, 30);
	test_eq("2000.05.31", 2000, 5, 31);
	test_eq("2000.06.30", 2000, 6, 30);
	test_eq("2000.07.31", 2000, 7, 31);
	test_eq("2000.08.31", 2000, 8, 31);
	test_eq("2000.09.30", 2000, 9, 30);
	test_eq("2000.10.31", 2000, 10, 31);
	test_eq("2000.11.30", 2000, 11, 30);
	test_eq("2000.12.31", 2000, 12, 31);
}

#define INVALID_FOR(month) "Day is invalid for " month

void test_limit_errors() {
	test_err("2020.01.32", INVALID_FOR("January"));
	test_err("2020.02.30", INVALID_FOR("February"));
	test_err("2020.03.32", INVALID_FOR("March"));
	test_err("2020.04.31", INVALID_FOR("April"));
	test_err("2020.05.32", INVALID_FOR("May"));
	test_err("2020.06.31", INVALID_FOR("June"));
	test_err("2020.07.32", INVALID_FOR("July"));
	test_err("2020.08.32", INVALID_FOR("August"));
	test_err("2020.09.31", INVALID_FOR("September"));
	test_err("2020.10.32", INVALID_FOR("October"));
	test_err("2020.11.31", INVALID_FOR("November"));
	test_err("2020.12.32", INVALID_FOR("December"));
}

void feb_date(char *buffer, int year, int day) {
	memset(buffer, ' ', 10);
	sprintf(buffer, "%d.02.%d", year, day);
}

void test_leap_years() {
	int leap_years[] = {1600, 1804, 1808, 1948, 2000, 2004, 0}; 
	int non_leap[] = {1793, 1900, 2013, 0};
	for (int i = 0; leap_years[i]; i++) {
		char buf[11];
		feb_date(buf, leap_years[i], 28);
		test_eq(buf, leap_years[i], 02, 28);
		feb_date(buf, leap_years[i], 29);
		test_eq(buf, leap_years[i], 02, 29);
		feb_date(buf, leap_years[i], 30);
		test_err(buf, INVALID_FOR("February"));
	}
	for (int i = 0; non_leap[i]; i++) {
		char buf[11];
		feb_date(buf, non_leap[i], 28);
		test_eq(buf, non_leap[i], 02, 28);
		feb_date(buf, non_leap[i], 29);
		test_err(buf, INVALID_FOR("February"));
	}
}

void test_errors() {
	test_err(0, "No date specified");
	test_err("2014-30", "Date must be exactly 10 characters");
	test_err("2014412345", "No month specified");
	test_err("2014412.45", "No day specified");
	test_err("20144.2.45", "Year must be exactly 4 digits");
	test_err("2014.2.3..", "Month must be exactly 2 digits");
	test_err("2014.45.1.", "Day must be exactly 2 digits");
	test_err("-001.12.05", "Year must be positive");
 	test_err("1000.-1.05", "Month must be positive");
	test_err("2020.01.00", "Day must be positive");
	test_err("2020.13.04", "Month cannot be greater than 12");
}


int main(int argc, char **argv) {
	test_basic();
	test_limits();
	test_errors();
	test_limit_errors();
	test_leap_years();
	printf("All %d tests passed\n", tests); 
}
