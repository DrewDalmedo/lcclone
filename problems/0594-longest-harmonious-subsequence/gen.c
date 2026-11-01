/* Small cases have up to 10 elements, drawn from at most five values,
 * spaced 1 apart (now and then 2 apart), mostly near 0, else at the
 * bottom or the top of the range or anywhere in it, so that equal
 * values, neighbours and gaps of 2 all come up. Full-size cases have
 * 2 * 10^4 elements, in a shape the seed picks: all different and never
 * 1 apart (answer 0: the slowest input for counting each element against
 * all the others, and too many values for a small fixed table); all
 * different, spread over the whole range; many copies of a few dozen
 * values; only two values, 1 apart, at an end of the range; or all
 * equal. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 10) : 20000, spread, step, where, base, off, i;
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small) {
        spread = 0;
        if (!rnd_chance(10))
            spread = (int)rnd(1, 4);
        step = rnd_chance(20) ? 2 : 1;
        where = (int)rnd(0, 4);
        if (where <= 1)
            base = (int)rnd(-3, 3);
        else if (where == 2)
            base = -1000000000;
        else
            base = (int)rnd(-1000000000, 1000000000 - 8);
        for (i = 0; i < n; i++) {
            off = step * (int)rnd(0, spread);
            nums[i] = where == 3 ? 1000000000 - off : base + off;
        }
    } else {
        switch (gen_seed() % 5) {
        case 0: /* all different, and all an even distance apart */
            for (i = 0; i < n; i++) {
                off = (int)rnd(0, 49999);
                nums[i] = (int)(-1000000000
                                + 2 * ((long long)i * 50000 + off));
            }
            rnd_shuffle(nums, n);
            break;
        case 1: /* all different, over the whole range */
            for (i = 0; i < n; i++) {
                off = (int)rnd(0, 99999);
                nums[i] = (int)(-1000000000 + (long long)i * 100000 + off);
            }
            rnd_shuffle(nums, n);
            break;
        case 2: /* many copies of a few dozen neighbouring values */
            base = (int)rnd(-1000000000, 1000000000 - 40);
            for (i = 0; i < n; i++)
                nums[i] = base + (int)rnd(0, 40);
            break;
        case 3: /* two values 1 apart, at the bottom or the top */
            base = rnd_chance(50) ? -1000000000 : 1000000000 - 1;
            for (i = 0; i < n; i++)
                nums[i] = base + (int)rnd(0, 1);
            break;
        default: /* all equal */
            base = (int)rnd(-1000000000, 1000000000);
            for (i = 0; i < n; i++)
                nums[i] = base;
            break;
        }
    }
    put_ints(nums, n);
    free(nums);
}
