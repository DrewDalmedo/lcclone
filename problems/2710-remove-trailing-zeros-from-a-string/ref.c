/* Reference solution: walk back from the end over the zeros. The first
 * digit is never 0, so the walk stops at the latest at the start; return
 * a copy of the prefix that ends at the last non-zero digit. */
char *removeTrailingZeros(char *num)
{
    size_t len = strlen(num);
    char *out;

    while (len > 1 && num[len - 1] == '0')
        len--;
    out = malloc(len + 1);
    memcpy(out, num, len);
    out[len] = '\0';
    return out;
}
