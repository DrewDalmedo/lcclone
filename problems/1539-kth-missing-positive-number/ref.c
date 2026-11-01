/* Reference solution: below arr[i] there are arr[i] - (i + 1) numbers not
 * in arr, a count that never falls as i grows. Binary search for the first
 * position p where it reaches k: the answer comes before arr[p], and k of
 * the numbers below it are missing and p are taken, so it is k + p. When
 * no position reaches k, p = n and the answer lies past the end. */
int findKthPositive(int *arr, int arrSize, int k)
{
    int lo = 0, hi = arrSize, mid;

    while (lo < hi) {
        mid = lo + (hi - lo) / 2;
        if (arr[mid] - (mid + 1) >= k)
            hi = mid;
        else
            lo = mid + 1;
    }
    return k + lo;
}
