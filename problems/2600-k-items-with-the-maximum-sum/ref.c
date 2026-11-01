/* Reference solution: greedy. Take +1 chips while there are any, then the
 * 0 chips, and only then the -1 chips, as many as k still demands. */
int kItemsWithMaximumSum(int numOnes, int numZeros, int numNegOnes, int k)
{
    int ones = k < numOnes ? k : numOnes;
    int forced = k - numOnes - numZeros;

    (void)numNegOnes;
    return forced > 0 ? ones - forced : ones;
}
