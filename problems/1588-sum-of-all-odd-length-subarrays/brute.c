/* Slow and obviously right: go through every window of odd length and
 * add up its elements from scratch. */
int sumOddLengthSubarrays(int *arr, int arrSize)
{
    int total = 0, len, l, i;

    for (len = 1; len <= arrSize; len += 2)
        for (l = 0; l + len <= arrSize; l++)
            for (i = l; i < l + len; i++)
                total += arr[i];
    return total;
}
