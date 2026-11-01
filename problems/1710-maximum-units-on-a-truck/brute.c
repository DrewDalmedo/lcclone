/* Slow and obviously right, without relying on the greedy choice: a 0/1
 * knapsack over the individual boxes, each taking one place on the truck.
 * best[j] is the most units that at most j of the boxes seen so far can
 * hold. */
int maximumUnits(int **boxTypes, int boxTypesSize, int *boxTypesColSize,
                 int truckSize)
{
    long long stock = 0;
    int cap, result, i, k, j, *best;

    (void)boxTypesColSize;
    for (i = 0; i < boxTypesSize; i++)
        stock += boxTypes[i][0];
    cap = stock < truckSize ? (int)stock : truckSize;
    best = calloc((size_t)cap + 1, sizeof *best);
    for (i = 0; i < boxTypesSize; i++)
        for (k = 0; k < boxTypes[i][0]; k++)
            for (j = cap; j >= 1; j--)
                if (best[j - 1] + boxTypes[i][1] > best[j])
                    best[j] = best[j - 1] + boxTypes[i][1];
    result = best[cap];
    free(best);
    return result;
}
