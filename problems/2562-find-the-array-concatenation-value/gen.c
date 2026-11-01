/* Values of every length from 1 to 5 digits, with powers of ten (10, 100,
 * ...) and 10^4 itself common, since they are where digit counting goes
 * wrong. */
static int value(int small)
{
    static const int tens[] = {1, 10, 100, 1000, 10000};
    int digits;

    if (rnd_chance(small ? 25 : 10))
        return tens[rnd(0, 4)];
    digits = (int)rnd(1, 4);
    return (int)rnd(1, tens[digits] - 1);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000, i;
    int nums[1000];

    for (i = 0; i < n; i++)
        nums[i] = value(small);
    put_ints(nums, n);
}
