/* Reference solution: simulate on a copy. While the array still drops
 * somewhere, find the leftmost neighbouring pair with the smallest sum
 * (a strict < keeps the first of equal sums), store the sum in the left
 * element and shift the rest left by one. At most n - 1 fusions of O(n)
 * each, O(n^2) in all. Every element is the sum of a run of inputs, so
 * it stays within 50 * 1000 in size and fits an int. */
int minimumPairRemoval(int *nums, int numsSize)
{
    int *a = malloc((size_t)numsSize * sizeof *a);
    int n = numsSize, fusions = 0, i, j, best;

    memcpy(a, nums, (size_t)n * sizeof *a);
    for (;;) {
        for (i = 1; i < n && a[i - 1] <= a[i]; i++)
            ;
        if (i >= n)
            break;
        best = 0;
        for (j = 1; j + 1 < n; j++)
            if (a[j] + a[j + 1] < a[best] + a[best + 1])
                best = j;
        a[best] += a[best + 1];
        for (j = best + 1; j + 1 < n; j++)
            a[j] = a[j + 1];
        n--;
        fusions++;
    }
    free(a);
    return fusions;
}
