/* Reference solution: simulate the passes on a copy of s. A block of r
 * digits adds up to at most 9r, which never takes more than r characters
 * to write, so each pass can write its totals over the string it is
 * reading: the write position never passes the read position. */
char *digitSum(char *s, int k)
{
    size_t n = strlen(s), i, j, len;
    char *cur = malloc(n + 1), digits[4];
    int sum, t;

    memcpy(cur, s, n + 1);
    while (n > (size_t)k) {
        len = 0;
        for (i = 0; i < n; i += (size_t)k) {
            sum = 0;
            for (j = i; j < n && j < i + (size_t)k; j++)
                sum += cur[j] - '0';
            t = 0;
            do {
                digits[t++] = (char)('0' + sum % 10);
                sum /= 10;
            } while (sum > 0);
            while (t > 0)
                cur[len++] = digits[--t];
        }
        cur[len] = '\0';
        n = len;
    }
    return cur;
}
