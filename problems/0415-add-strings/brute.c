/* Slow and obviously right: start from num1 and add num2 one unit at a
 * time. A digit d in column p of num2 means adding 10^p d times, and
 * adding 10^p is counting up from column p: 9s turn into 0s until a digit
 * below 9, which goes up by one. The digits are kept least significant
 * first and written out from the highest nonzero one ("0" if none). */
char *addStrings(char *num1, char *num2)
{
    int l1 = (int)strlen(num1), l2 = (int)strlen(num2);
    int n = (l1 > l2 ? l1 : l2) + 1, top, i, p, times;
    char *digits = calloc((size_t)n, 1);
    char *out;

    for (i = 0; i < l1; i++)
        digits[i] = (char)(num1[l1 - 1 - i] - '0');
    for (p = 0; p < l2; p++)
        for (times = num2[l2 - 1 - p] - '0'; times > 0; times--) {
            for (i = p; digits[i] == 9; i++)
                digits[i] = 0;
            digits[i]++;
        }
    for (top = n - 1; top > 0 && digits[top] == 0; top--)
        ;
    out = malloc((size_t)top + 2);
    for (i = 0; i <= top; i++)
        out[i] = (char)('0' + digits[top - i]);
    out[top + 1] = '\0';
    free(digits);
    return out;
}
