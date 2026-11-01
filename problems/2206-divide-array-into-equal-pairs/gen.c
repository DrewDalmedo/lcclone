/* An even-length array. Small cases have 2 to 8 elements of values up to
 * 4 (5 for a changed element), so that repeated values, counts of 3 and 4,
 * and both answers come up often; full-size ones have 1000 elements of
 * values up to 500, or up to a smaller cap so that values repeat many
 * times. The seed picks the shape: values in pairs (true), pairs with one
 * or two elements changed (usually false), or every value random. The
 * array is then shuffled. */
void gen(int small)
{
    int len = small ? 2 * (int)rnd(1, 4) : 1000, i, k;
    int top = small ? (int)rnd(1, 4) : rnd_chance(70) ? 500 : (int)rnd(1, 30);
    int shape = (int)rnd(0, 3), v, pos;
    int nums[1000];

    for (i = 0; i < len; i += 2) {
        v = (int)rnd(1, top);
        nums[i] = nums[i + 1] = v;
    }
    if (shape == 1 || shape == 2) {
        for (k = 0; k < shape; k++) {
            pos = (int)rnd(0, len - 1);
            v = (int)rnd(1, small ? top + 1 : 500);
            nums[pos] = v;
        }
    } else if (shape == 3) {
        for (i = 0; i < len; i++)
            nums[i] = (int)rnd(1, top);
    }
    rnd_shuffle(nums, len);
    put_ints(nums, len);
}
