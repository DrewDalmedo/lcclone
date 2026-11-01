/* Reference solution: a sliding window. For position 0 the window covers
 * code[1..k] when k > 0, or code[n+k..n-1] when k < 0; moving to the next
 * position drops the window's first number and adds the one after its
 * last, with every index taken modulo n. */
int *decrypt(int *code, int codeSize, int k, int *returnSize)
{
    int n = codeSize, lo, hi, sum = 0, i;
    int *res = malloc((size_t)n * sizeof *res);

    *returnSize = n;
    if (k == 0) {
        for (i = 0; i < n; i++)
            res[i] = 0;
        return res;
    }
    if (k > 0) {
        lo = 1;
        hi = k;
    } else {
        lo = n + k;
        hi = n - 1;
    }
    for (i = lo; i <= hi; i++)
        sum += code[i];
    for (i = 0; i < n; i++) {
        res[i] = sum;
        sum -= code[(lo + i) % n];
        sum += code[(hi + i + 1) % n];
    }
    return res;
}
