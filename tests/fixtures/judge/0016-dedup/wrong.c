/* Counts the distinct values but doesn't move them to the front. */
int dedup(int* nums, int numsSize) {
    int i, k = 0;
    for (i = 0; i < numsSize; i++)
        if (i == 0 || nums[i] != nums[i - 1])
            k++;
    return k;
}
