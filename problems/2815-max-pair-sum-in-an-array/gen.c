/* Values built with a chosen top digit, so that top digits repeat or all
 * differ as the seed decides. Small cases have 2 to 8 values of up to
 * three digits, from a few top digits, with copies of earlier values and
 * the odd 10000; full-size ones have 100 values, either any from 1 to
 * 10^4 or built like the small ones with up to four digits (and 10000
 * rarely or never, so that it doesn't decide every answer). */

/* A value of len digits (1 to 4) whose largest digit is top. */
static int with_top(int top, int len)
{
    int digit[4], v = 0, pos, j;

    for (j = 0; j < len; j++)
        digit[j] = (int)rnd(j == 0 ? 1 : 0, top);
    pos = (int)rnd(0, len - 1);
    digit[pos] = top;
    for (j = 0; j < len; j++)
        v = v * 10 + digit[j];
    return v;
}

void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 100;
    int keys = (int)rnd(2, 9);
    int any = !small && rnd_chance(50);
    int tens = small ? 4 : (int)rnd(0, 1);
    int nums[100], i, top, len, from;

    for (i = 0; i < n; i++) {
        if (any) {
            nums[i] = (int)rnd(1, 10000);
        } else if (rnd_chance(tens)) {
            nums[i] = 10000;
        } else if (i > 0 && rnd_chance(15)) {
            from = (int)rnd(0, i - 1);
            nums[i] = nums[from];
        } else {
            top = (int)rnd(1, keys);
            len = (int)rnd(1, small ? 3 : 4);
            nums[i] = with_top(top, len);
        }
    }
    put_ints(nums, n);
}
