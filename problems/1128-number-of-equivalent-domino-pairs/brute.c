/* Slow and obviously right: try every pair of positions i < j and test
 * the definition, the halves equal as they are or after a turn. */
int numEquivDominoPairs(int **dominoes, int dominoesSize,
                        int *dominoesColSize)
{
    int pairs = 0, i, j;

    (void)dominoesColSize;
    for (i = 0; i < dominoesSize; i++)
        for (j = i + 1; j < dominoesSize; j++) {
            int *p = dominoes[i], *q = dominoes[j];

            if ((p[0] == q[0] && p[1] == q[1]) ||
                (p[0] == q[1] && p[1] == q[0]))
                pairs++;
        }
    return pairs;
}
