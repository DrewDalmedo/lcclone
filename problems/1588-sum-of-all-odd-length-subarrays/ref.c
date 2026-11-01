/* Reference solution: arr[i] lies in (i + 1) * (n - i) windows, one for
 * each choice of start at or before i and end at or after i. Half of them,
 * rounded up, have odd length, so add arr[i] that many times. O(n). */
int sumOddLengthSubarrays(int *arr, int arrSize)
{
    int n = arrSize, total = 0, i;

    for (i = 0; i < n; i++)
        total += arr[i] * (((i + 1) * (n - i) + 1) / 2);
    return total;
}
