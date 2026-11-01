/* Slow and obviously right: search every way of matching the entries. The
 * first entry not used yet is either left out or matched with each later
 * unused entry that is its mirror, and the largest count wins. This relies
 * on nothing about how many mirrors an entry can have. */
static int used[50];

static int is_mirror(const char *x, const char *y)
{
    return x[0] == y[1] && x[1] == y[0];
}

static int best(char **words, int n, int i)
{
    int top, got, j;

    while (i < n && used[i])
        i++;
    if (i == n)
        return 0;
    used[i] = 1;
    top = best(words, n, i + 1);
    for (j = i + 1; j < n; j++)
        if (!used[j] && is_mirror(words[i], words[j])) {
            used[j] = 1;
            got = 1 + best(words, n, i + 1);
            used[j] = 0;
            if (got > top)
                top = got;
        }
    used[i] = 0;
    return top;
}

int maximumNumberOfStringPairs(char **words, int wordsSize)
{
    memset(used, 0, sizeof used);
    return best(words, wordsSize, 0);
}
