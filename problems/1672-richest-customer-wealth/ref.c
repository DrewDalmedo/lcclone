/* Reference solution: add up each row (one customer's money at every
 * bank) and keep the largest total. */
int maximumWealth(int **accounts, int accountsSize, int *accountsColSize)
{
    int best = 0, sum, i, j;

    for (i = 0; i < accountsSize; i++) {
        sum = 0;
        for (j = 0; j < accountsColSize[i]; j++)
            sum += accounts[i][j];
        if (sum > best)
            best = sum;
    }
    return best;
}
