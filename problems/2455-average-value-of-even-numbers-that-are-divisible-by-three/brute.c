/* Slow and obviously right: for every possible value from 1 to 1000 that is
 * even and divisible by 3, count its occurrences with a scan of nums and add
 * them in. Then find the rounded-down mean by counting up the largest q
 * with q * count <= sum, instead of dividing. */
int averageValue(int *nums, int numsSize)
{
    int sum = 0, count = 0, v, i, q;

    for (v = 1; v <= 1000; v++) {
        if (v % 2 != 0 || v % 3 != 0)
            continue;
        for (i = 0; i < numsSize; i++)
            if (nums[i] == v) {
                sum += v;
                count++;
            }
    }
    if (count == 0)
        return 0;
    q = 0;
    while ((q + 1) * count <= sum)
        q++;
    return q;
}
