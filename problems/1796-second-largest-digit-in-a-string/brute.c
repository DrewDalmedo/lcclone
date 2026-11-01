/* Slow and obviously right: try each value from 9 down, and return the
 * first one that appears in s while some bigger digit appears too. */
static int appears(const char *s, int d)
{
    for (; *s; s++)
        if (*s == '0' + d)
            return 1;
    return 0;
}

int secondHighest(char *s)
{
    int v, w;

    for (v = 9; v >= 0; v--) {
        if (!appears(s, v))
            continue;
        for (w = v + 1; w <= 9; w++)
            if (appears(s, w))
                return v;
    }
    return -1;
}
