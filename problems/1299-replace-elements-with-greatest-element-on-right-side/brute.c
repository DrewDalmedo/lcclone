/* Slow and obviously right: for each position, look through everything
 * after it. */
int *replaceElements(int *arr, int arrSize, int *returnSize)
{
    int *out = malloc((size_t)arrSize * sizeof *out);
    int i, j;

    for (i = 0; i < arrSize; i++) {
        out[i] = -1;
        for (j = i + 1; j < arrSize; j++)
            if (arr[j] > out[i])
                out[i] = arr[j];
    }
    *returnSize = arrSize;
    return out;
}
