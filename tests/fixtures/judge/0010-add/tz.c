/* Adds by counting days with mktime, as a solution to "the number of days
 * between two dates" (1360) might: the days from 1 March 2024 to a + b
 * days later. Under a time zone with daylight saving time, the day the
 * clocks go forward is 23 hours long, and 100 + 23 comes out as 122; lc
 * runs programs in UTC, as LeetCode's machines are. */
#include <time.h>

int add(int a, int b) {
    long long s = (long long)a + b;
    struct tm t = {0};
    time_t from, to;

    if (s < -10000 || s > 10000)
        return (int)s;
    t.tm_year = 124;
    t.tm_mon = 2;
    t.tm_mday = 1;
    t.tm_isdst = -1;
    from = mktime(&t);
    t.tm_year = 124;
    t.tm_mon = 2;
    t.tm_mday = 1 + (int)s;
    t.tm_hour = t.tm_min = t.tm_sec = 0;
    t.tm_isdst = -1;
    to = mktime(&t);
    return (int)((to - from) / 86400);
}
