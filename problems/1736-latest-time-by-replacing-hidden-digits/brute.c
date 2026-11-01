/* Slow and obviously right: try every time of day from 23:59 down to
 * 00:00 and return the first one that agrees with every readable digit. */
char *maximumTime(char *time)
{
    char *r = malloc(6);
    int t, i, ok;

    for (t = 23 * 60 + 59; t >= 0; t--) {
        sprintf(r, "%02d:%02d", t / 60, t % 60);
        ok = 1;
        for (i = 0; i < 5; i++)
            if (time[i] != '?' && time[i] != r[i])
                ok = 0;
        if (ok)
            return r;
    }
    return r;
}
