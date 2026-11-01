/* Slow and obviously right: do the two stages literally on a copy. The
 * merge stage visits each pair of neighbours in turn; the pack stage swaps
 * a zero with the non-zero value just after it until no such pair is left,
 * which moves the zeros back without reordering the other values. */
int *applyOperations(int *nums, int numsSize, int *returnSize)
{
    int *a = malloc((size_t)numsSize * sizeof *a);
    int i, swapped, t;

    for (i = 0; i < numsSize; i++)
        a[i] = nums[i];
    for (i = 0; i + 1 < numsSize; i++)
        if (a[i] == a[i + 1]) {
            a[i] *= 2;
            a[i + 1] = 0;
        }
    do {
        swapped = 0;
        for (i = 0; i + 1 < numsSize; i++)
            if (a[i] == 0 && a[i + 1] != 0) {
                t = a[i];
                a[i] = a[i + 1];
                a[i + 1] = t;
                swapped = 1;
            }
    } while (swapped);
    *returnSize = numsSize;
    return a;
}
