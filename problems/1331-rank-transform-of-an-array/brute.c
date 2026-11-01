/* Slow and obviously right: the rank of an element is 1 plus the number of
 * different values below it. Count them by checking every element that is
 * smaller and is the first occurrence of its value. */
int *arrayRankTransform(int *arr, int arrSize, int *returnSize)
{
    int *out = malloc((size_t)(arrSize > 0 ? arrSize : 1) * sizeof *out);
    int i, j, k, first, below;

    for (i = 0; i < arrSize; i++) {
        below = 0;
        for (j = 0; j < arrSize; j++) {
            if (arr[j] >= arr[i])
                continue;
            first = 1;
            for (k = 0; k < j; k++)
                if (arr[k] == arr[j])
                    first = 0;
            below += first;
        }
        out[i] = below + 1;
    }
    *returnSize = arrSize;
    return out;
}
