/* Slow and obviously right: for each element, compare it with every
 * element of the array. */
int *smallerNumbersThanCurrent(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int i, j;

    for (i = 0; i < numsSize; i++) {
        out[i] = 0;
        for (j = 0; j < numsSize; j++)
            if (nums[j] < nums[i])
                out[i]++;
    }
    *returnSize = numsSize;
    return out;
}
