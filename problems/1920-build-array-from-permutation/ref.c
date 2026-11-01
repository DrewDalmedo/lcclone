/* Reference solution, in place with O(1) extra memory: every slot keeps
 * its old value as the remainder modulo n and gains the new one as the
 * quotient, nums[i] += n * (nums[nums[i]] % n); a second pass divides by
 * n. The largest stored value is n * n - 1, below 10^6. */
int *buildArray(int *nums, int numsSize, int *returnSize)
{
    int n = numsSize, i;

    for (i = 0; i < n; i++)
        nums[i] += n * (nums[nums[i]] % n);
    for (i = 0; i < n; i++)
        nums[i] /= n;
    *returnSize = n;
    return nums;
}
