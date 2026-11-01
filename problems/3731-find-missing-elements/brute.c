/* Slow and obviously right: go through every allowed value v from 1 to
 * 100 and keep it when nums holds something smaller, something larger,
 * and not v itself. */
int *findMissingElements(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc(100 * sizeof *out);
    int m = 0, v, i, below, above, found;

    for (v = 1; v <= 100; v++) {
        below = above = found = 0;
        for (i = 0; i < numsSize; i++) {
            if (nums[i] < v)
                below = 1;
            if (nums[i] > v)
                above = 1;
            if (nums[i] == v)
                found = 1;
        }
        if (below && above && !found)
            out[m++] = v;
    }
    *returnSize = m;
    return out;
}
