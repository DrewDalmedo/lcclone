/* Reference solution: tally the digits by peeling them off with % 10,
 * then scan the values 0 to 9 upwards, skipping the ones with no tally
 * and keeping the first that beats the lowest tally so far. */
int getLeastFrequentDigit(int n)
{
    int tally[10] = {0}, best = -1, d;

    while (n > 0) {
        tally[n % 10]++;
        n /= 10;
    }
    for (d = 0; d < 10; d++)
        if (tally[d] > 0 && (best < 0 || tally[d] < tally[best]))
            best = d;
    return best;
}
