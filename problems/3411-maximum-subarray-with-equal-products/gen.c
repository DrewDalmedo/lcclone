/* Values from 1 to 10. Small cases have 2 to 8 elements, with many 1s or,
 * for some seeds, only even values (answer 2). Full-size cases have 100
 * elements, shaped by the seed: mostly 1s with a few other values (long
 * answers); only 2s, 8s or 10s, or any even values, whose long products
 * overflow 64 bits (answer 2); uniform values; or even values followed by
 * a stretch of 1s, 3s, 5s, 7s and 9s, so the answer is at the far end. */
void gen(int small)
{
    static const int evens[] = {2, 4, 6, 8, 10};
    static const int odds[] = {3, 5, 7, 9};
    int n = 100, shape = (int)rnd(0, 4), one = 2 * (int)rnd(1, 5);
    int cut = (int)rnd(50, 90), i, x;
    int *nums;

    if (small)
        n = rnd_chance(20) ? 2 : (int)rnd(3, 8);
    nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++) {
        if (small)
            x = shape == 1 ? evens[rnd(0, 4)]
                : rnd_chance(40) ? 1 : (int)rnd(2, 10);
        else if (shape == 0)
            x = rnd_chance(85) ? 1 : (int)rnd(2, 10);
        else if (shape == 1)
            x = one == 4 || one == 6 ? evens[rnd(0, 4)] : one;
        else if (shape == 2)
            x = (int)rnd(1, 10);
        else if (i < cut)
            x = evens[rnd(0, 4)];
        else
            x = rnd_chance(70) ? 1 : odds[rnd(0, 3)];
        nums[i] = x;
    }
    put_ints(nums, n);
    free(nums);
}
