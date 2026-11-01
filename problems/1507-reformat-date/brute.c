/* Slow and obviously right: write out every day and month the way the
 * input does ("1st Jan ", "2nd Jan ", ...) until one matches the start of
 * date, then print the year that follows with that month and day. */
static const char *suffix(int day)
{
    if (day == 1 || day == 21 || day == 31)
        return "st";
    if (day == 2 || day == 22)
        return "nd";
    if (day == 3 || day == 23)
        return "rd";
    return "th";
}

char *reformatDate(char *date)
{
    static const char *names[12] = {"Jan", "Feb", "Mar", "Apr", "May",
                                    "Jun", "Jul", "Aug", "Sep", "Oct",
                                    "Nov", "Dec"};
    char *out = malloc(11), prefix[16];
    int day, month, len;

    out[0] = '\0';
    for (month = 1; month <= 12; month++)
        for (day = 1; day <= 31; day++) {
            len = snprintf(prefix, sizeof prefix, "%d%s %s ", day,
                           suffix(day), names[month - 1]);
            if (strncmp(date, prefix, (size_t)len) == 0)
                snprintf(out, 11, "%.4s-%02d-%02d", date + len, month, day);
        }
    return out;
}
