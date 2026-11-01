/* Reference solution: mark which values occur in nums, then step through
 * k, 2k, 3k, ... while the current one is marked. No value above 100
 * occurs, so the walk stops at the first multiple past 100 without
 * reading the table there (the answer can reach 200). */
int missingMultiple(int *nums, int numsSize, int k)
{
    bool seen[101] = {false};
    int m = k, i;

    for (i = 0; i < numsSize; i++)
        seen[nums[i]] = true;
    while (m <= 100 && seen[m])
        m += k;
    return m;
}
