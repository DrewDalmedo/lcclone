/* Small cases are 1 to 8 values whose magnitudes come from a narrow range
 * (1 to 6, or the top end next to 1000), so that opposites, repeats and
 * near misses come up often; a quarter of them use a single sign. Full-size
 * ones are 1000 values, in a shape the seed picks: random signs; one sign
 * per magnitude, so nothing is mirrored; the same with one small mirrored
 * pair placed at the very end; or mostly unpartnered positives above 500
 * with a few small values of either sign. */
static int nums[1000];
static int sign_of[1001];

static int coin(void)
{
    return rnd_chance(50) ? 1 : -1;
}

void gen(int small)
{
    int n, i, top, base, fixed, mag, s, k, shape;

    if (small) {
        n = (int)rnd(1, 8);
        top = (int)rnd(1, 6);
        base = rnd_chance(20) ? 1000 - top : 0;
        fixed = rnd_chance(25) ? coin() : 0;
        for (i = 0; i < n; i++) {
            mag = base + (int)rnd(1, top);
            s = fixed ? fixed : coin();
            nums[i] = s * mag;
        }
        put_ints(nums, n);
        return;
    }
    n = 1000;
    shape = (int)(gen_seed() % 4);
    for (i = 1; i <= 1000; i++)
        sign_of[i] = coin();
    for (i = 0; i < n; i++) {
        if (shape == 3 && !rnd_chance(5)) {
            nums[i] = (int)rnd(501, 1000);
            continue;
        }
        mag = (int)rnd(1, shape == 3 ? 500 : 1000);
        s = shape == 1 || shape == 2 ? sign_of[mag] : coin();
        nums[i] = s * mag;
    }
    if (shape == 2) {
        k = (int)rnd(1, 50);
        nums[n - 2] = k;
        nums[n - 1] = -k;
    }
    put_ints(nums, n);
}
