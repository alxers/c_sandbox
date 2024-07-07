#include <stdio.h>

static char daytab[2][13] = {
  { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
  { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};


int day_of_year(int year, int month, int day) {
  int i, leap;

  char valid_year = year >= 0;
  char valid_month = month >= 1 && month <= 12;
  char valid_day = day >= 1 && day <= 31;

  if (!valid_day && !valid_month && !valid_year) {
    return 0;
  } 

  leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;

  for(i = 1; i < month; i++) {
    // day += daytab[leap][i];
    day += *(*(daytab+leap)+i);
  }

  return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday) {
  int i, leap;

  if (year < 0) {
    return;
  } 

  leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;

  for(i = 1; yearday > *(*(daytab+leap)+i); i++) {
    yearday -= *(*(daytab+leap)+i);
  }

  *pmonth = i;
  *pday = yearday;
}

int main() {
  printf("day: %d\n", day_of_year(2022, 2, 16));

  int pm = 0;
  int pd = 0;
  month_day(2022, 32, &pm, &pd);
  printf("Month: %d, day: %d\n", pm, pd);
  return 0;
}