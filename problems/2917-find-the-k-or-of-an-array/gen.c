/* Small: up to 8 elements. Most values use only the low 1 to 4 bits, so
 * the vote counts land near k (which leans to the lower half); some are
 * 0, 2^31 - 1, or have bit 30 set.
 * Full size: 50 elements, either fully random 31-bit values with k near
 * the middle (each bit then gets close to k votes), or copies of 2 to 4
 * random masks, some with bits cleared, with k anywhere. */
void gen(int small)
{
    int nums[50], masks[4];
    int n, k, i, t, w, kinds;

    n = small ? (int)rnd(1, 8) : 50;
    if (small) {
        w = (int)rnd(1, 4);
        for (i = 0; i < n; i++) {
            t = (int)rnd(0, 9);
            if (t == 0)
                nums[i] = 0;
            else if (t == 1)
                nums[i] = 2147483647;
            else if (t == 2)
                nums[i] = (1 << 30) | (int)rnd(0, (1 << w) - 1);
            else
                nums[i] = (int)rnd(0, (1 << w) - 1);
        }
        k = rnd_chance(50) ? (int)rnd(1, (n + 1) / 2) : (int)rnd(1, n);
    } else if (rnd_chance(50)) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(0, 2147483647);
        k = (int)rnd(20, 30);
    } else {
        kinds = (int)rnd(2, 4);
        for (t = 0; t < kinds; t++)
            masks[t] = (int)rnd(0, 2147483647);
        for (i = 0; i < n; i++) {
            t = (int)rnd(0, kinds - 1);
            nums[i] = masks[t];
            if (rnd_chance(30))
                nums[i] &= (int)rnd(0, 2147483647);
        }
        k = (int)rnd(1, n);
    }
    put_ints(nums, n);
    put_int(k);
}
