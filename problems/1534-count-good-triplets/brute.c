/* Slow and obviously right: try every i < j < k and test the three
 * limits. */
int countGoodTriplets(int *arr, int arrSize, int a, int b, int c)
{
    int i, j, k, count = 0;

    for (i = 0; i < arrSize; i++)
        for (j = i + 1; j < arrSize; j++)
            for (k = j + 1; k < arrSize; k++)
                if (abs(arr[i] - arr[j]) <= a && abs(arr[j] - arr[k]) <= b
                    && abs(arr[i] - arr[k]) <= c)
                    count++;
    return count;
}
