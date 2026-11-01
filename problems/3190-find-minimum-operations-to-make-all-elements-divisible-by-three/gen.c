/* Random elements. The seed picks which remainders mod 3 may appear (all,
 * only one, or two of them), so arrays needing only raises, only lowers
 * or nothing at all come up often. Small cases use a few low values;
 * full-size ones have 50 elements up to 50. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 50, i;
    int top = small ? (int)rnd(3, 12) : 50;
    int mask = (int)rnd(1, 7), r, q;
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++) {
        do
            r = (int)rnd(0, 2);
        while (!(mask >> r & 1));
        if (r == 0)
            q = (int)rnd(1, top / 3);
        else
            q = (int)rnd(0, (top - r) / 3);
        nums[i] = 3 * q + r;
    }
    put_ints(nums, n);
    free(nums);
}
