/* Small cases: n is 1 to 40 and values 1 to 20. k is mostly between 0
 * and the number of binary digits of n - 1 (the most 1 digits an index
 * can have is at most that), and now and then anything up to 10, so that
 * k = 0, k above every index's count and n next to a power of two all
 * come up. Full-size cases: n is 1000 or, now and then, less, and k is 0
 * to 10, mostly 3 to 6 where the most indices qualify; the values are
 * random up to 10^5, or now and then all 10^5, which makes the largest
 * totals. */
void gen(int small)
{
    int nums[1000];
    int n, k, top, all_max, digits, i;

    if (small) {
        n = (int)rnd(1, 40);
        for (digits = 0; (n - 1) >> digits; digits++)
            ;
        k = rnd_chance(15) ? (int)rnd(0, 10) : (int)rnd(0, digits);
        top = 20;
        all_max = 0;
    } else {
        n = rnd_chance(70) ? 1000 : (int)rnd(1, 999);
        k = rnd_chance(70) ? (int)rnd(3, 6) : (int)rnd(0, 10);
        top = 100000;
        all_max = rnd_chance(20);
    }
    for (i = 0; i < n; i++)
        nums[i] = all_max ? top : (int)rnd(1, top);
    put_ints(nums, n);
    put_int(k);
}
