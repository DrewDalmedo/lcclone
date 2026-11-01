/* Reference solution: two indices. k counts the distinct values kept so
 * far, at nums[0..k); since nums is sorted, an element is new exactly
 * when it differs from the last one kept, and then it is copied to
 * nums[k]. */
int removeDuplicates(int *nums, int numsSize)
{
    int i, k = 1;

    for (i = 1; i < numsSize; i++)
        if (nums[i] != nums[k - 1])
            nums[k++] = nums[i];
    return k;
}
