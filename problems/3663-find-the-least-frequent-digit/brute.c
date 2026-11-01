/* Slow and obviously right: write n out as text, then try every tally from
 * 1 upwards and, for each, every digit from 0 upwards, counting that
 * digit's places in the text anew. The first digit whose count matches
 * has the lowest tally and is the smallest such digit. */
int getLeastFrequentDigit(int n)
{
    char text[16];
    int len = snprintf(text, sizeof text, "%d", n), f, d, i, count;

    for (f = 1; f <= len; f++)
        for (d = 0; d < 10; d++) {
            count = 0;
            for (i = 0; i < len; i++)
                if (text[i] == '0' + d)
                    count++;
            if (count == f)
                return d;
        }
    return -1;
}
