/* Slow and obviously right: for each value of arr2 in turn, scan arr1 and
 * copy out every element equal to it. Then gather the elements of arr1
 * that are equal to no value of arr2 and sort them with a bubble sort. */
int *relativeSortArray(int *arr1, int arr1Size, int *arr2, int arr2Size,
                       int *returnSize)
{
    int *out = malloc((size_t)arr1Size * sizeof *out);
    int k = 0, start, listed, i, j, t;

    for (j = 0; j < arr2Size; j++)
        for (i = 0; i < arr1Size; i++)
            if (arr1[i] == arr2[j])
                out[k++] = arr1[i];
    start = k;
    for (i = 0; i < arr1Size; i++) {
        listed = 0;
        for (j = 0; j < arr2Size; j++)
            if (arr1[i] == arr2[j])
                listed = 1;
        if (!listed)
            out[k++] = arr1[i];
    }
    for (i = start; i < k; i++)
        for (j = start; j + 1 < k; j++)
            if (out[j] > out[j + 1]) {
                t = out[j];
                out[j] = out[j + 1];
                out[j + 1] = t;
            }
    *returnSize = arr1Size;
    return out;
}
