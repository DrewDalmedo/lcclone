/* Two numbers from 0 to 10^9. Small cases mostly use the low 8 switches,
 * so that zeros, equal numbers and near misses come up often; full-size
 * ones start from a number of 30 bits. The seed picks how goal is made
 * from start: by flipping a random number of random switches, a few of
 * them possibly above start's highest 1 bit (switch 29 is put back down
 * if that passes 10^9); as an unrelated number of any length; as start
 * itself; as 0; or by flipping every switch up to start's width (start is
 * then picked so that this stays within 10^9). Then the two may swap. */
static long long upto(int width)
{
    long long top = (1LL << width) - 1;

    return rnd(0, top < 1000000000 ? top : 1000000000);
}

void gen(int small)
{
    int shape = (int)rnd(0, 5);
    int width = !small ? 30 : rnd_chance(70) ? (int)rnd(1, 8)
              : (int)rnd(9, 30);
    int reach = width + 2 < 30 ? width + 2 : 30, flips, k, bit;
    long long full = (1LL << width) - 1, start, goal, t;

    if (shape == 5 && full > 1000000000)
        start = rnd(full - 1000000000, 1000000000);
    else
        start = upto(width);
    goal = start;
    if (shape <= 1) {
        flips = (int)rnd(1, width);
        for (k = 0; k < flips; k++) {
            bit = (int)rnd(0, reach - 1);
            goal ^= 1LL << bit;
        }
        if (goal > 1000000000)
            goal ^= 1LL << 29;
    } else if (shape == 2) {
        goal = upto((int)rnd(1, 30));
    } else if (shape == 4) {
        goal = 0;
    } else if (shape == 5) {
        goal = start ^ full;
    }
    if (rnd_chance(50)) {
        t = start;
        start = goal;
        goal = t;
    }
    put_int(start);
    put_int(goal);
}
