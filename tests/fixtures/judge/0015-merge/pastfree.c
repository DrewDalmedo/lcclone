/* Merges right and frees b, then writes one element past the end of a:
 * freeing one array doesn't hide a write past the end of another. */
void merge(int* a, int aSize, int m, int* b, int bSize, int n) {
    int i = m - 1, j = n - 1, k = m + n - 1;
    (void)bSize;
    while (j >= 0)
        a[k--] = i >= 0 && a[i] > b[j] ? a[i--] : b[j--];
    free(b);
    a[aSize] = 7;
}
