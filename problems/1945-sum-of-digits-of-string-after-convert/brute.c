/* Slow and obviously right: write the spelled-out number as a string of
 * digits, then fold k times, each time adding up the digit characters of
 * the current number written out in decimal. */
int getLucky(char *s, int k)
{
    char digits[256];
    int len = 0, value, i, j;

    for (i = 0; s[i] != '\0'; i++)
        len += sprintf(digits + len, "%d", s[i] - 'a' + 1);
    for (i = 0; i < k; i++) {
        value = 0;
        for (j = 0; j < len; j++)
            value += digits[j] - '0';
        len = sprintf(digits, "%d", value);
    }
    return atoi(digits);
}
