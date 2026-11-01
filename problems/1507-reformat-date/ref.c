/* Reference solution: read the day's digits up to its suffix, find the
 * month by comparing all three letters after the first space with the
 * twelve abbreviations, and copy the year, the last four characters. */
char *reformatDate(char *date)
{
    static const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May",
                                     "Jun", "Jul", "Aug", "Sep", "Oct",
                                     "Nov", "Dec"};
    char *out = malloc(11);
    const char *p = date, *year = date + strlen(date) - 4;
    int day = 0, month = 0;

    while (*p >= '0' && *p <= '9')
        day = day * 10 + (*p++ - '0');
    p = strchr(p, ' ') + 1;
    while (month < 12 && strncmp(p, months[month], 3) != 0)
        month++;
    memcpy(out, year, 4);
    out[4] = '-';
    out[5] = (char)('0' + (month + 1) / 10);
    out[6] = (char)('0' + (month + 1) % 10);
    out[7] = '-';
    out[8] = (char)('0' + day / 10);
    out[9] = (char)('0' + day % 10);
    out[10] = '\0';
    return out;
}
