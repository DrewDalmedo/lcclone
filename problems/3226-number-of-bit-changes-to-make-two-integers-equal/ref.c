/* Reference solution: a move can only clear a 1-bit of n, so k is
 * reachable exactly when every 1-bit of k is also set in n. The moves are
 * then forced: clear each bit set in n but not in k, which is the number
 * of 1-bits of n ^ k. */
int minChanges(int n, int k)
{
    int diff, count = 0;

    if ((n & k) != k)
        return -1;
    for (diff = n ^ k; diff != 0; diff &= diff - 1)
        count++;
    return count;
}
