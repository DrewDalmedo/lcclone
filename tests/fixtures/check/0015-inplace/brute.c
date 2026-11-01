/* Keeps the elements in increasing order instead. */
int drop(int *nums, int numsSize, int val)
{
    int i, j, k = 0, t;

    for (i = 0; i < numsSize; i++)
        if (nums[i] != val)
            nums[k++] = nums[i];
    for (i = 1; i < k; i++)
        for (j = i; j > 0 && nums[j - 1] > nums[j]; j--) {
            t = nums[j];
            nums[j] = nums[j - 1];
            nums[j - 1] = t;
        }
    return k;
}
