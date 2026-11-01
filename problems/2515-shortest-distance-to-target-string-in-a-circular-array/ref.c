/* Reference solution: for every index i whose word equals target, the
 * two ways round the table take d = |i - startIndex| and n - d moves;
 * keep the smallest of these over all such i. */
int closestTarget(char **words, int wordsSize, char *target, int startIndex)
{
    int best = -1, i, d;

    for (i = 0; i < wordsSize; i++)
        if (strcmp(words[i], target) == 0) {
            d = i > startIndex ? i - startIndex : startIndex - i;
            if (wordsSize - d < d)
                d = wordsSize - d;
            if (best < 0 || d < best)
                best = d;
        }
    return best;
}
