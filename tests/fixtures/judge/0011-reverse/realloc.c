/* Grows a to twice its size, as solutions to 1929 do with
 * `realloc(nums, 2 * n)`, fills the second half with the help of a
 * scratch array, then moves the answer to the front and returns a. The
 * solution may reallocate what it is given, as on LeetCode. The memory a
 * had is no longer a's: when realloc moves a, the scratch array may well
 * be placed there. */
int* reverse(int* a, int aSize, int* returnSize) {
    int *r = realloc(a, 2 * (aSize ? aSize : 1) * sizeof *r), *t;
    if (!r)
        return NULL;
    t = malloc((aSize + 1) * sizeof *t);
    for (int i = 0; i < aSize; i++)
        t[i] = r[aSize - 1 - i];
    t[aSize] = -1;
    memcpy(r + aSize, t, aSize * sizeof *r);
    memmove(r, r + aSize, aSize * sizeof *r);
    *returnSize = aSize;
    return r;
}
