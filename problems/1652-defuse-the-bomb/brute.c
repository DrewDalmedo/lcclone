/* Slow and obviously right: for each position, walk |k| steps around the
 * circle in k's direction and add up the numbers stepped on. */
int *decrypt(int *code, int codeSize, int k, int *returnSize)
{
    int n = codeSize, step = k > 0 ? 1 : -1, steps = k > 0 ? k : -k;
    int *res = malloc((size_t)n * sizeof *res), i, s, pos;

    for (i = 0; i < n; i++) {
        res[i] = 0;
        pos = i;
        for (s = 0; s < steps; s++) {
            pos = (pos + step + n) % n;
            res[i] += code[pos];
        }
    }
    *returnSize = n;
    return res;
}
