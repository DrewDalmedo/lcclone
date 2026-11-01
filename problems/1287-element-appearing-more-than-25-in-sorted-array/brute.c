/* Slow and obviously right: count the copies of each element's value
 * over the whole array, and return the first value whose count is more
 * than a quarter of it. */
int findSpecialInteger(int *arr, int arrSize)
{
    int i, j, count;

    for (i = 0; i < arrSize; i++) {
        count = 0;
        for (j = 0; j < arrSize; j++)
            if (arr[j] == arr[i])
                count++;
        if (4 * count > arrSize)
            return arr[i];
    }
    return -1;
}
