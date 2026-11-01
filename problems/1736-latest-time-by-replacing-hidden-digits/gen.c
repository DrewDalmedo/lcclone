/* A valid time of day with some of its four digits replaced by '?', so a
 * valid completion always exists. Which digits are hidden is random (each
 * of the 16 patterns as likely). The hour is often one of 03, 04, 13, 14,
 * 20, 23, 09 and 19: second hour digits of 3 and 4 are the boundary for a
 * hidden first digit. The input is always five characters, so full-size
 * cases are made the same way. */
void gen(int small)
{
    static const int hint[] = {3, 4, 13, 14, 23, 20, 9, 19};
    int hour, minute, mask, i;
    char s[6];

    (void)small;
    if (rnd_chance(40))
        hour = hint[rnd(0, 7)];
    else
        hour = (int)rnd(0, 23);
    minute = (int)rnd(0, 59);
    mask = (int)rnd(0, 15);
    s[0] = (char)('0' + hour / 10);
    s[1] = (char)('0' + hour % 10);
    s[2] = ':';
    s[3] = (char)('0' + minute / 10);
    s[4] = (char)('0' + minute % 10);
    s[5] = '\0';
    for (i = 0; i < 4; i++)
        if (mask & (1 << i))
            s[i < 2 ? i : i + 1] = '?';
    put_string(s);
}
