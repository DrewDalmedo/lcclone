/* Reference solution: keep the digits, then copy them out three at a time
 * while more than four are left; the last 2 or 3 make one group, and the
 * last 4 make two groups of 2. D digits give at most D + D / 3 characters,
 * which can be more than the input's length. */
char *reformatNumber(char *number)
{
    size_t len = strlen(number), d = 0, i, left, k = 0, at = 0;
    char *digits = malloc(len + 1);
    char *out;

    for (i = 0; i < len; i++)
        if (number[i] >= '0' && number[i] <= '9')
            digits[d++] = number[i];
    out = malloc(d + d / 3 + 1);
    left = d;
    while (left > 4) {
        memcpy(out + k, digits + at, 3);
        k += 3;
        at += 3;
        left -= 3;
        out[k++] = '-';
    }
    if (left == 4) {
        out[k++] = digits[at++];
        out[k++] = digits[at++];
        out[k++] = '-';
        left = 2;
    }
    memcpy(out + k, digits + at, left);
    k += left;
    out[k] = '\0';
    free(digits);
    return out;
}
