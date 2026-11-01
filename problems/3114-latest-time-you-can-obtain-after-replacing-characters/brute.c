/* Slow and obviously right: go through every time from 11:59 back to 00:00
 * and return the first one that has the same digit as s wherever s has a
 * digit. */
char *findLatestTime(char *s)
{
    char *t = malloc(6);
    int minutes, i, ok;

    for (minutes = 11 * 60 + 59; minutes >= 0; minutes--) {
        t[0] = (char)('0' + minutes / 60 / 10);
        t[1] = (char)('0' + minutes / 60 % 10);
        t[2] = ':';
        t[3] = (char)('0' + minutes % 60 / 10);
        t[4] = (char)('0' + minutes % 60 % 10);
        t[5] = '\0';
        ok = 1;
        for (i = 0; i < 5; i++)
            if (s[i] != '?' && s[i] != t[i])
                ok = 0;
        if (ok)
            return t;
    }
    t[0] = '\0';
    return t;
}
