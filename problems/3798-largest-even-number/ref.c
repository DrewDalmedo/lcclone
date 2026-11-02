/* Reference solution: no digit is 0, so a longer number is always a
 * larger one. Every even result ends at some 2, so the longest is s up to
 * and including its last 2: find that 2 scanning from the right and copy
 * the prefix ("" when there is no 2). */
char *largestEven(char *s)
{
    int n = (int)strlen(s), end = n;
    char *out;

    while (end > 0 && s[end - 1] != '2')
        end--;
    out = malloc((size_t)end + 1);
    memcpy(out, s, (size_t)end);
    out[end] = '\0';
    return out;
}
