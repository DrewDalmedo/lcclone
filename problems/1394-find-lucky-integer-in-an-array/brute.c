/* Slow and obviously right: for each entry, count the entries equal to it
 * by scanning the whole array, and keep the largest entry whose count is
 * the entry itself. */
int findLucky(int *arr, int arrSize)
{
    int best = -1, i, k, copies;

    for (i = 0; i < arrSize; i++) {
        copies = 0;
        for (k = 0; k < arrSize; k++)
            if (arr[k] == arr[i])
                copies++;
        if (copies == arr[i] && arr[i] > best)
            best = arr[i];
    }
    return best;
}
