/* Reference solution: add the columns from the right end of both strings
 * with a carry, writing the sum from the right end of a buffer one digit
 * longer than the longer input. When nothing carries out of the top column,
 * the spare first digit is dropped. */
char *addStrings(char *num1, char *num2)
{
    size_t i = strlen(num1), j = strlen(num2);
    size_t n = (i > j ? i : j) + 1, k = n;
    char *out = malloc(n + 1);
    int carry = 0, sum;

    out[n] = '\0';
    while (i > 0 || j > 0) {
        sum = carry;
        if (i > 0)
            sum += num1[--i] - '0';
        if (j > 0)
            sum += num2[--j] - '0';
        out[--k] = (char)('0' + sum % 10);
        carry = sum / 10;
    }
    if (carry) {
        out[0] = '1';
        return out;
    }
    memmove(out, out + 1, n);
    return out;
}
