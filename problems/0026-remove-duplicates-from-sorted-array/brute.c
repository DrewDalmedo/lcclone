/* Slow and obviously right: build the list of distinct values in a new
 * array, adding each element unless the list already has it (a linear
 * search, which doesn't rely on nums being sorted), then copy the list
 * back to the front of nums. */
int removeDuplicates(int *nums, int numsSize)
{
    int *seen = malloc((size_t)numsSize * sizeof *seen);
    int i, j, k = 0;

    for (i = 0; i < numsSize; i++) {
        for (j = 0; j < k; j++)
            if (seen[j] == nums[i])
                break;
        if (j == k)
            seen[k++] = nums[i];
    }
    for (j = 0; j < k; j++)
        nums[j] = seen[j];
    free(seen);
    return k;
}
