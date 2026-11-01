/* Reference solution: for each start, extend the substring one letter at a
 * time, keeping a bitmask of the lowercase letters it holds and one of the
 * uppercase letters. It is nice when the two masks are equal. Starts are
 * tried from the left and only a strictly longer nice substring replaces
 * the best one, so ties go to the leftmost. */
char *longestNiceSubstring(char *s)
{
    int n = (int)strlen(s), best = 0, from = 0, i, j;
    unsigned lower, upper;
    char *out;

    for (i = 0; i < n; i++) {
        lower = upper = 0;
        for (j = i; j < n; j++) {
            if (s[j] >= 'a' && s[j] <= 'z')
                lower |= 1u << (s[j] - 'a');
            else
                upper |= 1u << (s[j] - 'A');
            if (lower == upper && j - i + 1 > best) {
                best = j - i + 1;
                from = i;
            }
        }
    }
    out = malloc((size_t)best + 1);
    memcpy(out, s + from, (size_t)best);
    out[best] = '\0';
    return out;
}
