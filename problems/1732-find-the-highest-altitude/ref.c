/* Reference solution: walk the trail keeping the running altitude, and
 * the highest altitude so far, which starts at post 0's altitude, 0. */
int largestAltitude(int *gain, int gainSize)
{
    int alt = 0, best = 0, i;

    for (i = 0; i < gainSize; i++) {
        alt += gain[i];
        if (alt > best)
            best = alt;
    }
    return best;
}
