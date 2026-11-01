/* Slow and obviously right: count up from "0" in base 7, one step at a
 * time, until the count reaches |num|. A step adds 1 to the last digit;
 * a digit that passes 6 becomes 0 and carries 1 to the place on its left,
 * and a carry out of the first place adds a new digit 1 in front. */
char *convertToBase7(int num)
{
    char digits[16], *out;
    int len = 1, target = num < 0 ? -num : num, i, k;

    digits[0] = '0';
    for (k = 0; k < target; k++) {
        for (i = len - 1; i >= 0 && digits[i] == '6'; i--)
            digits[i] = '0';
        if (i >= 0) {
            digits[i]++;
        } else {
            memmove(digits + 1, digits, (size_t)len);
            digits[0] = '1';
            len++;
        }
    }
    out = malloc((size_t)len + 2);
    i = 0;
    if (num < 0)
        out[i++] = '-';
    memcpy(out + i, digits, (size_t)len);
    out[i + len] = '\0';
    return out;
}
