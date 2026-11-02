/* Random values. Small: 1 to 8 elements, mostly 1 to 8 so that views
 * stop at different places, now and then up to 1000. Full size: 100
 * elements; the seed picks values over the whole range, values 1 to 10
 * (short views), or values 99 to 1000, so that every view reaches back
 * to the start (the most work for a solution that walks each view). */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, i, lo = 1, hi, shape;
    int nums[100];

    shape = small ? 0 : (int)(gen_seed() % 3);
    if (small)
        hi = rnd_chance(80) ? 8 : 1000;
    else
        hi = shape == 1 ? 10 : 1000;
    if (shape == 2)
        lo = 99;
    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(lo, hi);
    put_ints(nums, n);
}
