/* The array is built from groups of equal values, each group's size a
 * multiple of k or not as the seed decides, and then shuffled. A value may
 * be drawn for two groups, which then merge. Small cases have one to eight
 * entries from 1 to 5, and k from 1 to 4 (sometimes up to 10, which can
 * exceed the length). Full-size ones have 100 entries, values from a
 * random range starting at 1 or ending at 100, and k from 1 to 12
 * (sometimes up to 100). */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100;
    int top = small ? 5 : (int)rnd(5, 100);
    int base = small || rnd_chance(50) ? 1 : 101 - top;
    int k, filled = 0, size, value, i;
    int nums[100];

    if (rnd_chance(15))
        k = small ? (int)rnd(1, 10) : (int)rnd(1, 100);
    else
        k = small ? (int)rnd(1, 4) : (int)rnd(1, 12);
    while (filled < n) {
        if (rnd_chance(60))
            size = k * (int)rnd(1, 3);
        else
            size = (int)rnd(1, small ? 4 : 2 * k + 1);
        if (size > n - filled)
            size = n - filled;
        value = (int)rnd(base, base + top - 1);
        for (i = 0; i < size; i++)
            nums[filled++] = value;
    }
    rnd_shuffle(nums, n);
    put_ints(nums, n);
    put_int(k);
}
