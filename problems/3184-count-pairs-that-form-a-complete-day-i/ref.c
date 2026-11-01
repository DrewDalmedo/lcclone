/* Reference solution: only the remainders modulo 24 matter. Go through the
 * jobs once, counting how many of the earlier ones have each remainder;
 * a job with remainder r makes a complete day with every earlier job of
 * remainder (24 - r) % 24. */
int countCompleteDayPairs(int *hours, int hoursSize)
{
    int seen[24] = {0};
    int pairs = 0, r, i;

    for (i = 0; i < hoursSize; i++) {
        r = hours[i] % 24;
        pairs += seen[(24 - r) % 24];
        seen[r]++;
    }
    return pairs;
}
