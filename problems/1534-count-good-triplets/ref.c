/* Reference solution: fix the middle position j, moving it left to right
 * while freq[] counts the values of arr[0..j-1], and turn freq into prefix
 * sums over the values 0..1000. For each k > j with arr[j] and arr[k]
 * within b, a fitting arr[i] lies within a of arr[j] and within c of
 * arr[k], a range of values whose count the prefix sums give at once.
 * O(n^2 + n * 1001) time. */
int countGoodTriplets(int *arr, int arrSize, int a, int b, int c)
{
    int freq[1001] = {0}, pre[1002];
    int j, k, v, lo, hi, count = 0;

    for (j = 0; j < arrSize; j++) {
        pre[0] = 0; /* pre[v] = how many of arr[0..j-1] are below v */
        for (v = 0; v <= 1000; v++)
            pre[v + 1] = pre[v] + freq[v];
        for (k = j + 1; k < arrSize; k++) {
            if (abs(arr[j] - arr[k]) > b)
                continue;
            lo = arr[j] - a > arr[k] - c ? arr[j] - a : arr[k] - c;
            hi = arr[j] + a < arr[k] + c ? arr[j] + a : arr[k] + c;
            if (lo < 0)
                lo = 0;
            if (hi > 1000)
                hi = 1000;
            if (lo <= hi)
                count += pre[hi + 1] - pre[lo];
        }
        freq[arr[j]]++;
    }
    return count;
}
