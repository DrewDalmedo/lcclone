/* Slow and obviously right: for every post, compare it with each post
 * before it and with each post after it. */
int *findValidElements(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int m = 0, i, k, left, right;

    for (i = 0; i < numsSize; i++) {
        left = 1;
        for (k = 0; k < i; k++)
            if (nums[k] >= nums[i])
                left = 0;
        right = 1;
        for (k = i + 1; k < numsSize; k++)
            if (nums[k] >= nums[i])
                right = 0;
        if (left || right)
            out[m++] = nums[i];
    }
    *returnSize = m;
    return out;
}
