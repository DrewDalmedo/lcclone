/* Reference solution: turn each tile so its smaller number comes first,
 * and count the tiles of each kind in a 10 x 10 table. A new tile pairs
 * with every equivalent tile seen before it, so add that count before
 * counting the tile itself. */
int numEquivDominoPairs(int **dominoes, int dominoesSize,
                        int *dominoesColSize)
{
    int count[10][10] = {{0}};
    int pairs = 0, i, a, b, t;

    (void)dominoesColSize;
    for (i = 0; i < dominoesSize; i++) {
        a = dominoes[i][0];
        b = dominoes[i][1];
        if (a > b) {
            t = a;
            a = b;
            b = t;
        }
        pairs += count[a][b];
        count[a][b]++;
    }
    return pairs;
}
