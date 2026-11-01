/* Slow and obviously right: add up every pair of positions i < j and keep
 * the sums that are a multiple of 24. */
int countCompleteDayPairs(int *hours, int hoursSize)
{
    int pairs = 0, i, j;

    for (i = 0; i < hoursSize; i++)
        for (j = i + 1; j < hoursSize; j++)
            if (((long long)hours[i] + hours[j]) % 24 == 0)
                pairs++;
    return pairs;
}
