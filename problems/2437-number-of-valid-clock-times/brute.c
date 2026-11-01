/* Slow and obviously right: write out each of the 1440 times of day and
 * count the ones that agree with the pattern at every fixed digit. */
int countTime(char *time)
{
    char t[6];
    int h, m, i, ok, count = 0;

    for (h = 0; h < 24; h++)
        for (m = 0; m < 60; m++) {
            t[0] = (char)('0' + h / 10);
            t[1] = (char)('0' + h % 10);
            t[2] = ':';
            t[3] = (char)('0' + m / 10);
            t[4] = (char)('0' + m % 10);
            ok = 1;
            for (i = 0; i < 5; i++)
                if (time[i] != '?' && time[i] != t[i])
                    ok = 0;
            count += ok;
        }
    return count;
}
