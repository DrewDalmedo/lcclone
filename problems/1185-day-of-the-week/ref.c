/* Reference solution: Sakamoto's method. Counting January and February
 * as part of the year before puts each leap day at the end of a year;
 * then the year, its leap years so far, a fixed shift for each month and
 * the day, added up modulo 7, give the weekday (0 for Sunday). */
char *dayOfTheWeek(int day, int month, int year)
{
    static const int shift[12] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    static const char *const names[7] = {
        "Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"
    };
    int y = month < 3 ? year - 1 : year;
    const char *name = names[(y + y / 4 - y / 100 + y / 400 + shift[month - 1] + day) % 7];
    char *out = malloc(strlen(name) + 1);

    strcpy(out, name);
    return out;
}
