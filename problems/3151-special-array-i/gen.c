/* Values that alternate in parity from a random start, then the seed picks
 * what to spoil: nothing, one element given the other parity (often the
 * last one in full-size cases), or every element drawn at random. Small
 * cases have 1 to 6 values up to a small top; full-size ones 100 values
 * up to 100. */
static int with_parity(int odd, int top)
{
    if (odd)
        return 2 * (int)rnd(0, (top - 1) / 2) + 1;
    return 2 * (int)rnd(1, top / 2);
}

void gen(int small)
{
    int nums[100], n, top, i, start, shape;

    n = small ? (int)rnd(1, 6) : 100;
    top = small ? (int)rnd(2, 12) : 100;
    start = (int)rnd(0, 1);
    for (i = 0; i < n; i++)
        nums[i] = with_parity((i + start) % 2, top);
    shape = (int)rnd(0, 2);
    if (shape == 1) {
        i = !small && rnd_chance(50) ? n - 1 : (int)rnd(0, n - 1);
        nums[i] = with_parity(nums[i] % 2 == 0, top);
    } else if (shape == 2) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, top);
    }
    put_ints(nums, n);
}
