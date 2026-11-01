/* nums and k. Small cases: 1 to 8 values from -5 to 5, so that zeros,
 * repeats and pairs x and -x come up often, and k from 1 to 10, so that k
 * can be below, equal to or above the number of negatives. The seed may
 * make every value negative, every value positive, or leave out 0.
 * Full-size cases: 10^4 values from -100 to 100 and k = 10^4, by seed
 * mostly negative, mostly positive, or all negative. */
void gen(int small)
{
    int n, k, top, shape, v, i;
    int *nums;

    if (small) {
        n = (int)rnd(1, 8);
        k = (int)rnd(1, 10);
        top = 5;
        shape = (int)rnd(0, 3);
    } else {
        n = 10000;
        k = 10000;
        top = 100;
        shape = 4 + (int)(gen_seed() % 3);
    }
    nums = malloc((size_t)n * sizeof *nums);
    for (i = 0; i < n; i++) {
        switch (shape) {
        case 0:                         /* anything, 0 more often */
            v = (int)rnd(-top, top);
            if (rnd_chance(15))
                v = 0;
            break;
        case 1:                         /* all negative */
        case 6:
            v = -(int)rnd(1, top);
            break;
        case 2:                         /* all positive */
            v = (int)rnd(1, top);
            break;
        case 3:                         /* no zero */
            v = (int)rnd(1, top);
            if (rnd_chance(50))
                v = -v;
            break;
        case 4:                         /* mostly negative */
            v = (int)rnd(0, top);
            if (rnd_chance(90))
                v = -v;
            break;
        default:                        /* mostly positive */
            v = (int)rnd(0, top);
            if (rnd_chance(10))
                v = -v;
            break;
        }
        nums[i] = v;
    }
    put_ints(nums, n);
    put_int(k);
    free(nums);
}
