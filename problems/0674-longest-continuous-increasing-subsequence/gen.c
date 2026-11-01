/* Small cases have up to 10 elements, in a shape the seed picks: a few
 * small values (so equal neighbours are common); a walk of small steps,
 * up, down or flat; values next to -10^9 and 10^9; or mostly climbing
 * with a few falls. Full-size cases have 10^4 elements: all climbing
 * (the slowest input for checking every block, or every start); random,
 * then a fall and a long climb to the end; random over the whole range;
 * all equal or all falling; or climbs of one length over and over. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 10) : 10000, lo, top, step, len, i;
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small) {
        switch (rnd(0, 3)) {
        case 0: /* a few small values */
            lo = (int)rnd(-5, 5);
            top = (int)rnd(1, 6);
            for (i = 0; i < n; i++)
                nums[i] = lo + (int)rnd(0, top);
            break;
        case 1: /* a walk: steps from -2 to 3 */
            nums[0] = (int)rnd(-5, 5);
            for (i = 1; i < n; i++) {
                step = (int)rnd(-2, 3);
                nums[i] = nums[i - 1] + step;
            }
            break;
        case 2: /* next to the bounds */
            for (i = 0; i < n; i++) {
                step = (int)rnd(0, 2);
                if (rnd_chance(50))
                    nums[i] = -1000000000 + step;
                else
                    nums[i] = 1000000000 - step;
            }
            break;
        default: /* mostly climbing, with a few falls or flat steps */
            nums[0] = (int)rnd(-10, 10);
            for (i = 1; i < n; i++) {
                if (rnd_chance(80))
                    step = (int)rnd(1, 3);
                else
                    step = -(int)rnd(0, 3);
                nums[i] = nums[i - 1] + step;
            }
            break;
        }
    } else {
        switch (gen_seed() % 5) {
        case 0: /* the whole array climbs */
            nums[0] = (int)rnd(-1000000000, -1000000000 + 1000);
            for (i = 1; i < n; i++) {
                step = (int)rnd(1, 199999);
                nums[i] = nums[i - 1] + step;
            }
            break;
        case 1: /* random, then a fall and the longest climb at the end */
            len = (int)rnd(1000, 5000);
            for (i = 0; i < n - len; i++)
                nums[i] = (int)rnd(-1000000000, 1000000000);
            nums[n - len] = -1000000000;
            for (i = n - len + 1; i < n; i++) {
                step = (int)rnd(1, 1000);
                nums[i] = nums[i - 1] + step;
            }
            break;
        case 2: /* random over the whole range */
            for (i = 0; i < n; i++)
                nums[i] = (int)rnd(-1000000000, 1000000000);
            break;
        case 3: /* all equal, or falling all the way */
            nums[0] = (int)rnd(0, 1000000000);
            step = (int)rnd(0, 1);
            for (i = 1; i < n; i++)
                nums[i] = nums[i - 1] - step * 100000;
            break;
        default: /* climbs of len elements, again and again */
            len = (int)rnd(2, 50);
            lo = (int)rnd(-1000000000, 1000000000 - 50);
            for (i = 0; i < n; i++)
                nums[i] = lo + i % len;
            break;
        }
    }
    put_ints(nums, n);
    free(nums);
}
