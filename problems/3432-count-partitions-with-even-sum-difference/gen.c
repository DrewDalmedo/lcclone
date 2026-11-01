/* Random passenger counts. Small: 2 to 8 carriages with 1 to 9
 * passengers each (now and then up to 100), sometimes all odd. Full
 * size: 100 carriages of 1 to 100; the seed picks whether the total
 * comes out even or odd. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 100, i, top, odd, total = 0, want;
    int nums[100];

    top = small && rnd_chance(80) ? 9 : 100;
    odd = rnd_chance(small ? 20 : 25);
    for (i = 0; i < n; i++) {
        nums[i] = (int)rnd(1, top);
        if (odd && nums[i] % 2 == 0)
            nums[i] = nums[i] == top ? nums[i] - 1 : nums[i] + 1;
        total += nums[i];
    }
    if (!small && !odd) {
        /* Set the parity of the total from the seed. */
        want = (int)(gen_seed() % 2);
        if (total % 2 != want) {
            i = (int)rnd(0, n - 1);
            nums[i] += nums[i] == 100 ? -1 : 1;
        }
    }
    put_ints(nums, n);
}
