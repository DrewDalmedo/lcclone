/* Slow and obviously right: take out each element in turn, copy the others
 * into a new array, and compare every pair of neighbours in it. */
bool canBeIncreasing(int *nums, int numsSize)
{
    int *rest = malloc((size_t)numsSize * sizeof *rest);
    bool found = false, ok;
    int k, i, m;

    for (k = 0; k < numsSize && !found; k++) {
        m = 0;
        for (i = 0; i < numsSize; i++)
            if (i != k)
                rest[m++] = nums[i];
        ok = true;
        for (i = 1; i < m; i++)
            if (rest[i - 1] >= rest[i])
                ok = false;
        found = ok;
    }
    free(rest);
    return found;
}
