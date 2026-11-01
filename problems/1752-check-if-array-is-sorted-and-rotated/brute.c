/* Slow and obviously right: sort a copy of nums, then try every cut of the
 * sorted copy (back piece first) and compare the result with nums. */
bool check(int *nums, int numsSize)
{
    int *sorted = malloc((size_t)numsSize * sizeof *sorted);
    bool found = false;
    int i, j, cut, v;

    for (i = 0; i < numsSize; i++) {
        v = nums[i];
        for (j = i; j > 0 && sorted[j - 1] > v; j--)
            sorted[j] = sorted[j - 1];
        sorted[j] = v;
    }
    for (cut = 0; cut < numsSize && !found; cut++) {
        found = true;
        for (i = 0; i < numsSize; i++)
            if (nums[i] != sorted[(cut + i) % numsSize])
                found = false;
    }
    free(sorted);
    return found;
}
