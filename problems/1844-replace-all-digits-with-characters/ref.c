/* Reference solution: copy s, and at each odd index add the digit's value
 * to the letter just before it. */
char *replaceDigits(char *s)
{
    size_t n = strlen(s), i;
    char *out = malloc(n + 1);

    memcpy(out, s, n + 1);
    for (i = 1; i < n; i += 2)
        out[i] = (char)(s[i - 1] + (s[i] - '0'));
    return out;
}
