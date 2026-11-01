/* Reference solution: counting sort. Count each value of arr1 in a table
 * of 1001 slots; write out the counted copies of each value of arr2 in
 * arr2's order, zeroing their counts, then whatever is left in the table
 * from value 0 up to 1000. */
int *relativeSortArray(int *arr1, int arr1Size, int *arr2, int arr2Size,
                       int *returnSize)
{
    int count[1001] = {0};
    int *out = malloc((size_t)arr1Size * sizeof *out);
    int k = 0, i, v;

    for (i = 0; i < arr1Size; i++)
        count[arr1[i]]++;
    for (i = 0; i < arr2Size; i++) {
        v = arr2[i];
        while (count[v] > 0) {
            out[k++] = v;
            count[v]--;
        }
    }
    for (v = 0; v <= 1000; v++)
        while (count[v] > 0) {
            out[k++] = v;
            count[v]--;
        }
    *returnSize = arr1Size;
    return out;
}
