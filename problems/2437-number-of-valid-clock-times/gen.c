/* Starts from a real time of day and hides a random set of its four digits
 * behind '?' (each of the 16 sets as likely), so some time always matches.
 * Half the hours come from a short list around the boundaries: units digits
 * 3 and 4 (with or without a 2 in front) and tens digit 2. The input is
 * always five characters, so full-size cases are made the same way. */
void gen(int small)
{
    static const int edge[] = {3, 4, 13, 14, 20, 23, 0, 19, 9, 10};
    char s[6];
    int hour, minute, hide, i;

    (void)small;
    if (rnd_chance(50))
        hour = edge[rnd(0, 9)];
    else
        hour = (int)rnd(0, 23);
    minute = (int)rnd(0, 59);
    hide = (int)rnd(0, 15);
    s[0] = (char)('0' + hour / 10);
    s[1] = (char)('0' + hour % 10);
    s[2] = ':';
    s[3] = (char)('0' + minute / 10);
    s[4] = (char)('0' + minute % 10);
    s[5] = '\0';
    for (i = 0; i < 4; i++)
        if (hide >> i & 1)
            s[i < 2 ? i : i + 1] = '?';
    put_string(s);
}
