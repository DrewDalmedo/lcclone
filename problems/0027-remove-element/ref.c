/* Reference solution: two indices. k counts the elements kept so far, at
 * nums[0..k); each element that isn't val is copied to nums[k]. The kept
 * elements stay in their original order. */
int removeElement(int *nums, int numsSize, int val)
{
    int i, k = 0;

    for (i = 0; i < numsSize; i++)
        if (nums[i] != val)
            nums[k++] = nums[i];
    return k;
}
