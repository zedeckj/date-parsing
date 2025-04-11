#ifndef DATEHEADER
#define DATEHEADER
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
typedef struct {
	int known_year : 1;
	u_int16_t year;
	
	int known_month : 1;
	u_int8_t month;
	
	int known_day : 1;
	u_int8_t day;
} date_t;

// Parses a string specifying a date in the format of "YYYY<sep>MM<sep>DD".
// Returns 0 if properly parsed, else an error message 
const char *parse_date(date_t *dst, char *str, char sep); 

#endif
