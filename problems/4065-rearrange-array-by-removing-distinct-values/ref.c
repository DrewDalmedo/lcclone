/* Reference solution: count each value. A value that occurs c times is
 * taken out in each of the first c rounds, so round k is every value with
 * a count of at least k, in increasing order; write the rounds one after
 * another, for k = 1 up to the largest count. */
int *rearrangeArray(int *nums, int numsSize, int *returnSize)
{
    int count[101] = {0}, rounds = 0, len = 0, i, k, v;
    int *ans = malloc((size_t)numsSize * sizeof *ans);

    for (i = 0; i < numsSize; i++)
        if (++count[nums[i]] > rounds)
            rounds = count[nums[i]];
    for (k = 1; k <= rounds; k++)
        for (v = 1; v <= 100; v++)
            if (count[v] >= k)
                ans[len++] = v;
    *returnSize = len;
    return ans;
}
