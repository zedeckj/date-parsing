#include "date.h"

#define INVALID_FOR(month) "Day is invalid for " month
#define ERROR(m) { err = m; goto res; }
/*
    January: 31 days
    February: 28 days and 29 in every leap year
    March: 31 days
    April: 30 days
    May: 31 days
    June: 30 days
    July: 31 days
    August: 31 days
    September: 30 days
    October: 31 days
    November: 30 days
    December: 31 days
*/

const char *validate(int year, int month, int day) {
	if (year < 1) return "Year must be positive";
	if (month < 1) return "Month must be positive";
	if (day < 1) return "Day must be positive";
	if (month > 12) return "Month cannot be greater than 12";
	static const int max_days[12] = {31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int max;
	if (month == 2) {	
		if (!(year % 400) || (!(year % 4) && year % 100)) max = 29;
		else max = 28;
		if (day > max) return INVALID_FOR("February");
	}
	else {
		max = max_days[month - 1];
		if (day > max) {
			switch (month) {
				case 1:
				return INVALID_FOR("January");
				case 3:
				return INVALID_FOR("March");
				case 4:
				return INVALID_FOR("April");
				case 5:
				return INVALID_FOR("May");
				case 6:
				return INVALID_FOR("June");
				case 7:
				return INVALID_FOR("July");
				case 8:
				return INVALID_FOR("August");
				case 9:
				return INVALID_FOR("September");
				case 10:
				return INVALID_FOR("October");
				case 11:
				return INVALID_FOR("November");
				case 12:
				return INVALID_FOR("December");
			}
		}	
	}
	return 0;
}

const char *parse_split(date_t *dst, char *ystr, char *mstr, char *dstr) {
	int year;
	int month; 
	int day;
	if (!sscanf(ystr, "%d", &year)) return "Year is not a number";
	if (!sscanf(mstr, "%d", &month)) return "Month is not a number";
	if (!sscanf(dstr, "%d", &day)) return "Day is not a number";
	const char *err = validate(year, month, day);
	if (err) return err;
	dst->year = year;
	dst->month = month;
	dst->day = day;
	return 0;
}

const char *parse_date(date_t *dst, char *in_str, char sep) {
	if (!in_str) return "No date specified";
	size_t len = strlen(in_str);
	if (len != 10) return "Date must be exactly 10 characters";
	const char *err = 0;
	char *buffer = malloc(strlen(in_str) + 1);
	strcpy(buffer, in_str);
	char *rest;
	char sep_str[2];
	sep_str[0] = sep;
	sep_str[1] = 0;
	char *year_str = strtok_r(buffer, sep_str, &rest);
	char *month_str = strtok_r(NULL, sep_str, &rest);
	if (!month_str) ERROR("No month specified");
	char *day_str = strtok_r(NULL, sep_str, &rest);
	if (!day_str) ERROR("No day specified"); 
	if (strlen(year_str) != 4) ERROR("Year must be exactly 4 digits");
	if (strlen(month_str) != 2) ERROR("Month must be exactly 2 digits");
	if (strlen(day_str) != 2) ERROR("Day must be exactly 2 digits");
	err = parse_split(dst, year_str, month_str, day_str);
	res:
	free(buffer);
	return err;

}
