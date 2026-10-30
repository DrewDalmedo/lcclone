int* reverse(int* a, int aSize, int* returnSize) {
    int *r = malloc((aSize ? aSize : 1) * sizeof *r);
    for (int i = 0; i < aSize; i++)
        r[i] = a[aSize - 1 - i];
    *returnSize = aSize;
    return r;
}
