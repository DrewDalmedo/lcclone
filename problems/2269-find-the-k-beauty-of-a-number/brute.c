/* Slow and obviously right: write num out as a string, and read the k
 * characters at each start position as a number, digit by digit. */
int divisorSubstrings(int num, int k)
{
    char s[16];
    int len = snprintf(s, sizeof s, "%d", num), count = 0, i, j;
    long long v;

    for (i = 0; i + k <= len; i++) {
        v = 0;
        for (j = i; j < i + k; j++)
            v = v * 10 + (s[j] - '0');
        if (v != 0 && num % v == 0)
            count++;
    }
    return count;
}
