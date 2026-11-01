int dedup(int* nums, int numsSize) {
    int i, k = 0;
    for (i = 0; i < numsSize; i++)
        if (k == 0 || nums[i] != nums[k - 1])
            nums[k++] = nums[i];
    return k;
}
