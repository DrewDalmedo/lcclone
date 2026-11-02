/* Reference solution: mark which of the values 1..100 occur in nums, then
 * start at the first whole number above the average, never below 1, and
 * step up past the marked values. For sum > 0 that start is
 * floor(sum / n) + 1, which C's division gives; for sum <= 0 the average
 * is at most 0, so every x >= 1 is above it. 101 is never marked, so the
 * walk stops there at the latest. */
int smallestAbsent(int *nums, int numsSize)
{
    char seen[102] = {0};
    int sum = 0, x, i;

    for (i = 0; i < numsSize; i++) {
        sum += nums[i];
        if (nums[i] >= 1)
            seen[nums[i]] = 1;
    }
    x = sum > 0 ? sum / numsSize + 1 : 1;
    while (seen[x])
        x++;
    return x;
}
