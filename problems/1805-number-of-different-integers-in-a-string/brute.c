/* Slow and obviously right: two digit strings spell the same integer when
 * they agree digit for digit once the shorter one is padded with zeros on
 * the left. Count the numbers of word that equal no number before them. */
static char digit_from_right(const char *p, int len, int k)
{
    return k < len ? p[len - 1 - k] : '0';
}

static int same(const char *a, int alen, const char *b, int blen)
{
    int m = alen > blen ? alen : blen, k;

    for (k = 0; k < m; k++)
        if (digit_from_right(a, alen, k) != digit_from_right(b, blen, k))
            return 0;
    return 1;
}

int numDifferentIntegers(char *word)
{
    int n = (int)strlen(word), runs = 0, count = 0, i, j, seen;
    int *start = malloc(((size_t)n + 1) * sizeof *start);
    int *len = malloc(((size_t)n + 1) * sizeof *len);

    for (i = 0; i < n; i++) {
        if (word[i] < '0' || word[i] > '9')
            continue;
        if (i > 0 && word[i - 1] >= '0' && word[i - 1] <= '9')
            len[runs - 1]++;
        else {
            start[runs] = i;
            len[runs] = 1;
            runs++;
        }
    }
    for (i = 0; i < runs; i++) {
        seen = 0;
        for (j = 0; j < i && !seen; j++)
            seen = same(word + start[i], len[i], word + start[j], len[j]);
        if (!seen)
            count++;
    }
    free(start);
    free(len);
    return count;
}
