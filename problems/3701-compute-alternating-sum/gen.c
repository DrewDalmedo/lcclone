/* Random values. Small cases have one to eight elements from 1 to 10, so
 * that equal pairs and zero totals come up. Full-size ones have 99 or 100
 * elements, and the seed picks the shape: random values, or the even
 * indices drawn high and the odd ones low, or the other way round (the
 * answer then lies near one end of its range). */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : (int)rnd(99, 100), i;
    int shape = small ? 0 : (int)rnd(0, 2);
    int nums[100];

    for (i = 0; i < n; i++) {
        if (small)
            nums[i] = (int)rnd(1, 10);
        else if (shape == 0)
            nums[i] = (int)rnd(1, 100);
        else if ((i % 2 == 0) == (shape == 1))
            nums[i] = (int)rnd(80, 100);
        else
            nums[i] = (int)rnd(1, 20);
    }
    put_ints(nums, n);
}
