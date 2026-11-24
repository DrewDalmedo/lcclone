/* Right, but too slow at full size: for each prefix that sums to a third of
 * the total, look through the later prefixes for one that sums to two
 * thirds. On the full-size case where a third keeps coming back and two
 * thirds never does, that is about 6 * 10^8 steps. */
bool canThreePartsEqualSum(int *arr, int arrSize)
{
    long long *p = malloc((size_t)arrSize * sizeof *p), total = 0;
    int i, j;

    for (i = 0; i < arrSize; i++) {
        total += arr[i];
        p[i] = total;
    }
    for (i = 0; i < arrSize - 2; i++) {
        if (p[i] * 3 != total)
            continue;
        for (j = i + 1; j < arrSize - 1; j++)
            if (p[j] * 3 == 2 * total) {
                free(p);
                return true;
            }
    }
    free(p);
    return false;
}
