/* Right, but too slow at full size: load one box at a time, each time
 * searching every kind for the most units among those with boxes left.
 * That is O(truckSize * m): about 10^9 steps for a 10^6-box truck and
 * 1000 kinds. */
int maximumUnits(int **boxTypes, int boxTypesSize, int *boxTypesColSize,
                 int truckSize)
{
    int *left = malloc((size_t)boxTypesSize * sizeof *left);
    int total = 0, i, best, t;

    (void)boxTypesColSize;
    for (i = 0; i < boxTypesSize; i++)
        left[i] = boxTypes[i][0];
    for (t = 0; t < truckSize; t++) {
        best = -1;
        for (i = 0; i < boxTypesSize; i++)
            if (left[i] > 0 &&
                (best < 0 || boxTypes[i][1] > boxTypes[best][1]))
                best = i;
        if (best < 0)
            break;
        left[best]--;
        total += boxTypes[best][1];
    }
    free(left);
    return total;
}
