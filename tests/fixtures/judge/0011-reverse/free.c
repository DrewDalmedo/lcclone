/* Frees a once it has the answer, then returns the answer in a new array
 * of one element more than a, which the allocator may well place where a
 * was: whatever it now holds past a's end is not a's. */
int* reverse(int* a, int aSize, int* returnSize) {
    int *t = malloc((aSize ? aSize : 1) * sizeof *t), *r;
    for (int i = 0; i < aSize; i++)
        t[i] = a[aSize - 1 - i];
    free(a);
    r = malloc((aSize + 1) * sizeof *r);
    memcpy(r, t, aSize * sizeof *r);
    r[aSize] = -1;
    free(t);
    *returnSize = aSize;
    return r;
}
