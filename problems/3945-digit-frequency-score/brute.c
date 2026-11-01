/* Slow and obviously right: write n out as a decimal string, then for each
 * digit d from 0 to 9 scan the whole string to count its copies, and add
 * d times that count. */
int digitFrequencyScore(int n)
{
    char text[16];
    int total = 0, d, c, i;

    snprintf(text, sizeof text, "%d", n);
    for (d = 0; d <= 9; d++) {
        c = 0;
        for (i = 0; text[i] != '\0'; i++)
            if (text[i] == '0' + d)
                c++;
        total += d * c;
    }
    return total;
}
