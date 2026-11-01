/* Slow and obviously right: best[i] is the most pieces the first i letters
 * can be cut into, or -1 if they can't be cut into balanced pieces at all.
 * For every end i, try every start j of the last piece, counting the
 * letters of s[j..i) one by one. */
int balancedStringSplit(char *s)
{
    int n = (int)strlen(s), i, j, ls, rs, answer;
    int *best = malloc(((size_t)n + 1) * sizeof *best);

    best[0] = 0;
    for (i = 1; i <= n; i++) {
        best[i] = -1;
        ls = rs = 0;
        for (j = i - 1; j >= 0; j--) {
            if (s[j] == 'L')
                ls++;
            else
                rs++;
            if (ls == rs && best[j] >= 0 && best[j] + 1 > best[i])
                best[i] = best[j] + 1;
        }
    }
    answer = best[n];
    free(best);
    return answer;
}
