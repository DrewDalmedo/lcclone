/* Reference solution: chain[i] is the number of copies of word in the
 * longest gapless chain that ends at index i of sequence (0 when word does
 * not end there). If word ends at i, chain[i] = chain[i - m] + 1, where m
 * is word's length; the answer is the largest chain[i]. Every end index is
 * tried, so chains that begin inside another occurrence are found too. */
int maxRepeating(char *sequence, char *word)
{
    int n = (int)strlen(sequence), m = (int)strlen(word);
    int *chain, i, best = 0;

    if (m > n)
        return 0;
    chain = calloc((size_t)n, sizeof *chain);
    for (i = m - 1; i < n; i++)
        if (memcmp(sequence + i - m + 1, word, (size_t)m) == 0) {
            chain[i] = (i >= m ? chain[i - m] : 0) + 1;
            if (chain[i] > best)
                best = chain[i];
        }
    free(chain);
    return best;
}
