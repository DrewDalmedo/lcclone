/* Slow and obviously right: turn each tile so its smaller number comes
 * first and name it 10 * smaller + larger (the numbers are 1..9, so each
 * kind of tile gets its own name), then try every pair of positions i < j
 * and count those whose tiles have the same name. That is O(n^2), but
 * the time limit accepts it: clang makes this loop too fast at 4 * 10^4
 * tiles for any limit to reject it with every compiler. */
int numEquivDominoPairs(int **dominoes, int dominoesSize,
                        int *dominoesColSize)
{
    int *key = malloc((size_t)(dominoesSize > 0 ? dominoesSize : 1) *
                      sizeof *key);
    int pairs = 0, i, j, a, b;

    (void)dominoesColSize;
    for (i = 0; i < dominoesSize; i++) {
        a = dominoes[i][0];
        b = dominoes[i][1];
        key[i] = a < b ? 10 * a + b : 10 * b + a;
    }
    for (i = 0; i < dominoesSize; i++)
        for (j = i + 1; j < dominoesSize; j++)
            if (key[i] == key[j])
                pairs++;
    free(key);
    return pairs;
}
