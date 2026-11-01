/* Starts from a time between 00:00 and 11:59 and blanks out a random set of
 * its four digits, so the blanks can always be filled in. Half the time the
 * hour is 00, 01, 02, 09, 10 or 11, whose second digits (0, 1, 2, 9) sit on
 * either side of the limit for a blank first digit. The input has a fixed
 * size, so full-size cases are made the same way. */
void gen(int small)
{
    static const int edge[] = {0, 1, 2, 9, 10, 11};
    char s[6];
    int hour, minute, i;

    (void)small;
    if (rnd_chance(50))
        hour = edge[rnd(0, 5)];
    else
        hour = (int)rnd(0, 11);
    minute = (int)rnd(0, 59);
    s[0] = (char)('0' + hour / 10);
    s[1] = (char)('0' + hour % 10);
    s[2] = ':';
    s[3] = (char)('0' + minute / 10);
    s[4] = (char)('0' + minute % 10);
    s[5] = '\0';
    for (i = 0; i < 5; i++)
        if (i != 2 && rnd_chance(50))
            s[i] = '?';
    put_string(s);
}
