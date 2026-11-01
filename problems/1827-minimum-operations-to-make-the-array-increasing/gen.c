/* Random stack heights. Small cases: a few heights from 1 to 6, so equal
 * neighbours are common. Full-size ones: 5000 stacks, in a shape the seed
 * picks: random heights; 10^4 first and every other stack 1 (the largest
 * answer, 62482501); all 10^4; or a row that climbs already except for
 * a random drop near the end. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 5000, i, shape = (int)rnd(0, 3), at, h;
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++) {
        if (small)
            nums[i] = (int)rnd(1, 6);
        else if (shape == 0)
            nums[i] = (int)rnd(1, 10000);
        else if (shape == 1)
            nums[i] = i == 0 ? 10000 : 1;
        else if (shape == 2)
            nums[i] = 10000;
        else
            nums[i] = 1 + i;
    }
    if (!small && shape == 3) {
        at = (int)rnd(n - 20, n - 1);
        h = (int)rnd(1, 10000);
        nums[at] = h;
    }
    put_ints(nums, n);
    free(nums);
}
