/* Slow and obviously right: try every multiple of 10 from 0 to 100 and
 * keep the one nearest to the price; on a tie the later, larger multiple
 * wins, since the comparison allows equality. */
int accountBalanceAfterPurchase(int purchaseAmount)
{
    int best = 0, m;

    for (m = 0; m <= 100; m += 10)
        if (abs(m - purchaseAmount) <= abs(best - purchaseAmount))
            best = m;
    return 100 - best;
}
