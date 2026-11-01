/* Slow and obviously right: erase each copy of digit in turn and keep the
 * largest number left. All of them have the same length and no zeros, so
 * the largest is the one that comes last in dictionary order. */
char *removeDigit(char *number, char digit)
{
    size_t n = strlen(number), i;
    char *best = NULL, *cand;

    for (i = 0; i < n; i++) {
        if (number[i] != digit)
            continue;
        cand = malloc(n);
        memcpy(cand, number, i);
        memcpy(cand + i, number + i + 1, n - i - 1);
        cand[n - 1] = '\0';
        if (!best || strcmp(cand, best) > 0) {
            free(best);
            best = cand;
        } else {
            free(cand);
        }
    }
    return best;
}
