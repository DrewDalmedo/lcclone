/* Right, but too slow at full size: sort a copy by insertion, drop its
 * repeats, and find each rank by binary search. The insertion sort is
 * O(n^2) unless the values come nearly in order: about 2.5 * 10^9 moves
 * for 10^5 values in random order, and twice that in descending order. */
int *arrayRankTransform(int *arr, int arrSize, int *returnSize)
{
    int *s = malloc((size_t)(arrSize + 1) * sizeof *s);
    int *out = malloc((size_t)(arrSize + 1) * sizeof *out);
    int d = 0, i, j, v, lo, hi, mid;

    for (i = 0; i < arrSize; i++) {
        v = arr[i];
        for (j = i; j > 0 && s[j - 1] > v; j--)
            s[j] = s[j - 1];
        s[j] = v;
    }
    for (i = 0; i < arrSize; i++)
        if (d == 0 || s[i] != s[d - 1])
            s[d++] = s[i];
    for (i = 0; i < arrSize; i++) {
        lo = 0;
        hi = d - 1;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (s[mid] < arr[i])
                lo = mid + 1;
            else
                hi = mid;
        }
        out[i] = lo + 1;
    }
    free(s);
    *returnSize = arrSize;
    return out;
}
