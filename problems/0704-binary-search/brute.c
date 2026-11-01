/* Slow and obviously right: compare target with every element in turn. */
int search(int *nums, int numsSize, int target)
{
    int i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] == target)
            return i;
    return -1;
}
