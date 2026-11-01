/* Small cases: a few moves, usually of -9 to 9 so that the sum crosses
 * zero and repeats, sometimes of up to 10^6 either way. Full size: 1000
 * moves, in a shape the seed picks: any lengths; long moves to the right
 * only (the sum climbs to nearly 10^9); long moves to the left only (it
 * sinks to nearly -10^9); or moves of exactly 10^6 or -10^6. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000;
    int *nums = malloc((size_t)n * sizeof *nums);
    int shape = small ? 0 : (int)rnd(0, 3);
    int top = 1000000, i;

    if (small)
        top = rnd_chance(70) ? 9 : 1000000;
    for (i = 0; i < n; i++) {
        if (shape == 0)
            nums[i] = (int)rnd(-top, top);
        else if (shape == 1)
            nums[i] = (int)rnd(990000, 1000000);
        else if (shape == 2)
            nums[i] = (int)rnd(-1000000, -990000);
        else
            nums[i] = rnd_chance(50) ? 1000000 : -1000000;
    }
    put_ints(nums, n);
    free(nums);
}
