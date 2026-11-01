/* Slow and obviously right: write every number out as text, add up its
 * digit characters, sort the sums and return the first. */
static int ascending(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int minElement(int *nums, int numsSize)
{
    int *sums = malloc((size_t)numsSize * sizeof *sums), i, best;
    char text[16], *p;

    for (i = 0; i < numsSize; i++) {
        sprintf(text, "%d", nums[i]);
        sums[i] = 0;
        for (p = text; *p; p++)
            sums[i] += *p - '0';
    }
    qsort(sums, (size_t)numsSize, sizeof *sums, ascending);
    best = sums[0];
    free(sums);
    return best;
}
