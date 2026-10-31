/* Slow and obviously right: build the new contents in a second array,
 * reading the original from the left and writing each zero twice until
 * the array is full, then copy it back. */
void duplicateZeros(int *arr, int arrSize)
{
    int *out = malloc((size_t)arrSize * sizeof *out);
    int r = 0, w = 0;

    while (w < arrSize) {
        out[w++] = arr[r];
        if (arr[r] == 0 && w < arrSize)
            out[w++] = 0;
        r++;
    }
    for (w = 0; w < arrSize; w++)
        arr[w] = out[w];
    free(out);
}
