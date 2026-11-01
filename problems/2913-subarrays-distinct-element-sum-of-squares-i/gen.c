/* Small: up to 8 numbers drawn from a pool of 1 to 6 values, so repeats
 * are common, either at random or in runs. A pool value may be 1, 100, or
 * 64 more or less than the one before it (a 64-bit mask indexed by value
 * mixes those up). Full size: 100 numbers, either 1..100 each once in a
 * random order (the largest answers), all at random, in long runs, or from
 * a pool of 2 to 10 values. */
void gen(int small)
{
    int nums[100], pool[10];
    int n, kinds, shape, i, t, v, len;

    shape = (int)rnd(0, 3);
    n = small ? (int)rnd(1, 8) : 100;
    kinds = small ? (int)rnd(1, 6) : (int)rnd(2, 10);
    for (i = 0; i < kinds; i++) {
        t = (int)rnd(0, 7);
        if (t == 0)
            pool[i] = 1;
        else if (t == 1)
            pool[i] = 100;
        else if (t == 2 && i > 0 && pool[i - 1] <= 36)
            pool[i] = pool[i - 1] + 64;
        else if (t == 2 && i > 0 && pool[i - 1] > 64)
            pool[i] = pool[i - 1] - 64;
        else
            pool[i] = (int)rnd(1, 100);
    }
    if (!small && shape == 0) {
        for (i = 0; i < n; i++)
            nums[i] = i + 1;
        rnd_shuffle(nums, n);
    } else if (!small && shape == 1) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, 100);
    } else if (shape == 2) {
        i = 0;
        while (i < n) {
            t = (int)rnd(0, kinds - 1);
            v = pool[t];
            len = (int)rnd(1, small ? 4 : 30);
            for (; len > 0 && i < n; len--)
                nums[i++] = v;
        }
    } else {
        for (i = 0; i < n; i++) {
            t = (int)rnd(0, kinds - 1);
            nums[i] = pool[t];
        }
    }
    put_ints(nums, n);
}
