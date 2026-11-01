/* Slow and obviously right: compare every entry of arr1 with every entry
 * of arr2, straight from the definition. */
int findTheDistanceValue(int *arr1, int arr1Size, int *arr2, int arr2Size,
                         int d)
{
    int count = 0, i, j, clear;

    for (i = 0; i < arr1Size; i++) {
        clear = 1;
        for (j = 0; j < arr2Size; j++)
            if (abs(arr1[i] - arr2[j]) <= d)
                clear = 0;
        count += clear;
    }
    return count;
}
