/* Slow and obviously right, from the definition of the richest customer:
 * go through the customers and return the wealth of the first one whom no
 * other customer beats, working out every wealth afresh each time it is
 * compared. */
static int wealth(int **accounts, int *accountsColSize, int i)
{
    int total = 0, j;

    for (j = 0; j < accountsColSize[i]; j++)
        total += accounts[i][j];
    return total;
}

int maximumWealth(int **accounts, int accountsSize, int *accountsColSize)
{
    int i, k, beaten;

    for (i = 0; i < accountsSize; i++) {
        beaten = 0;
        for (k = 0; k < accountsSize; k++)
            if (wealth(accounts, accountsColSize, k) >
                wealth(accounts, accountsColSize, i))
                beaten = 1;
        if (!beaten)
            return wealth(accounts, accountsColSize, i);
    }
    return 0;
}
