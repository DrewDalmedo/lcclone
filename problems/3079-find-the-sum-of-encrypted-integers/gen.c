/* Numbers from 1 to 1000. Each number is drawn from a random digit
 * count (1 to 3 digits, or 1000 itself), often with zeros in it, so that
 * short numbers, zeros and the one four-digit number all come up. Small
 * cases have up to 8 numbers; full-size cases have 50, and the seed may
 * make them all 1000 (the largest sum). */
static int number(void)
{
    int kind = (int)rnd(0, 9), digits, x, k, d;

    if (kind == 0)
        return 1000;
    digits = kind <= 3 ? 1 : kind <= 6 ? 2 : 3;
    x = (int)rnd(1, 9);
    for (k = 1; k < digits; k++) {
        d = rnd_chance(35) ? 0 : (int)rnd(0, 9);
        x = x * 10 + d;
    }
    return x;
}

void gen(int small)
{
    int nums[50];
    int n = small ? (int)rnd(1, 8) : 50;
    int all_max = !small && rnd_chance(25), i;

    for (i = 0; i < n; i++)
        nums[i] = all_max ? 1000 : number();
    put_ints(nums, n);
}
