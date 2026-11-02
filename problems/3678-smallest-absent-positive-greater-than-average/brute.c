/* Slow and obviously right: try x = 1, 2, 3, ... in turn and return the
 * first one that is above the average (x * n > sum, which is x > sum / n
 * without fractions) and equal to no entry of nums. */
int smallestAbsent(int *nums, int numsSize)
{
    long long sum = 0;
    int x, i, found;

    for (i = 0; i < numsSize; i++)
        sum += nums[i];
    for (x = 1;; x++) {
        if ((long long)x * numsSize <= sum)
            continue;
        found = 0;
        for (i = 0; i < numsSize; i++)
            if (nums[i] == x)
                found = 1;
        if (!found)
            return x;
    }
}
