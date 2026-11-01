/* Reference solution: the rounds take the values in ascending order, two
 * at a time. So sort a copy of nums, then swap the elements of each pair
 * (0, 1), (2, 3), ... so that the larger of each pair comes first. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int *numberGame(int *nums, int numsSize, int *returnSize)
{
    int *arr = malloc((size_t)numsSize * sizeof *arr);
    int i, t;

    memcpy(arr, nums, (size_t)numsSize * sizeof *arr);
    qsort(arr, (size_t)numsSize, sizeof *arr, cmp_int);
    for (i = 0; i + 1 < numsSize; i += 2) {
        t = arr[i];
        arr[i] = arr[i + 1];
        arr[i + 1] = t;
    }
    *returnSize = numsSize;
    return arr;
}
