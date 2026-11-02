/* Merges into a after reallocating it. a after the call is the result,
 * and the harness still has the old pointer: on LeetCode that is a
 * heap-use-after-free, a Runtime Error. */
void merge(int* a, int aSize, int m, int* b, int bSize, int n) {
    int i = m - 1, j = n - 1, k = m + n - 1;
    (void)bSize;
    a = realloc(a, (aSize + 1) * sizeof *a);
    while (j >= 0)
        a[k--] = i >= 0 && a[i] > b[j] ? a[i--] : b[j--];
}
