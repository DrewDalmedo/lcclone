/* Moves the last element into each gap, so the order changes. */
int drop(int *nums, int numsSize, int val)
{
    int i = 0, n = numsSize;

    while (i < n)
        if (nums[i] == val)
            nums[i] = nums[--n];
        else
            i++;
    return n;
}
