/* Slow and obviously right: for every slice, count the positions whose
 * value does not appear earlier in the same slice, with no table. */
int sumCounts(int *nums, int numsSize)
{
    int total = 0, i, j, p, q, d, first;

    for (i = 0; i < numsSize; i++)
        for (j = i; j < numsSize; j++) {
            d = 0;
            for (p = i; p <= j; p++) {
                first = 1;
                for (q = i; q < p; q++)
                    if (nums[q] == nums[p])
                        first = 0;
                d += first;
            }
            total += d * d;
        }
    return total;
}
