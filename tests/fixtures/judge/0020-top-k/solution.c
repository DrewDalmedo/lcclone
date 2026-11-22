static int desc(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x < y) - (x > y);
}

int *topK(int *nums, int numsSize, int k, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);

    memcpy(out, nums, (size_t)numsSize * sizeof *out);
    qsort(out, (size_t)numsSize, sizeof *out, desc);
    *returnSize = k;
    return out;
}
