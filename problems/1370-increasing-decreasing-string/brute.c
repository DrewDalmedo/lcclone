/* Slow and obviously right: mark the letters of s as used one by one,
 * following the picks literally. Going up, take the smallest unused
 * letter, then again and again the smallest unused letter greater than
 * the one just taken, until there is none; going down, the same with the
 * largest unused letter and smaller letters. Alternate until all are
 * used. Each pick scans the whole of s. */
char *sortString(char *s)
{
    int n = (int)strlen(s), k = 0, i, best, last, up;
    char *used = calloc((size_t)n + 1, 1);
    char *out = malloc((size_t)n + 1);

    while (k < n) {
        for (up = 1; up >= 0; up--) {
            last = -1;                  /* nothing taken yet this pass */
            for (;;) {
                best = -1;
                for (i = 0; i < n; i++) {
                    if (used[i])
                        continue;
                    if (up && (last < 0 || s[i] > last) &&
                        (best < 0 || s[i] < s[best]))
                        best = i;
                    if (!up && (last < 0 || s[i] < last) &&
                        (best < 0 || s[i] > s[best]))
                        best = i;
                }
                if (best < 0)
                    break;
                used[best] = 1;
                out[k++] = s[best];
                last = s[best];
            }
        }
    }
    out[k] = '\0';
    free(used);
    return out;
}
