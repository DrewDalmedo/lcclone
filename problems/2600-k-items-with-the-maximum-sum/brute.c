/* Slow and obviously right: try every way to split k into a count of +1
 * chips, a count of 0 chips and a count of -1 chips that the pile can
 * supply, and keep the best score. */
int kItemsWithMaximumSum(int numOnes, int numZeros, int numNegOnes, int k)
{
    int best = 0, found = 0, a, b, c;

    for (a = 0; a <= numOnes; a++)
        for (b = 0; b <= numZeros; b++) {
            c = k - a - b;
            if (c < 0 || c > numNegOnes)
                continue;
            if (!found || a - c > best)
                best = a - c;
            found = 1;
        }
    return best;
}
