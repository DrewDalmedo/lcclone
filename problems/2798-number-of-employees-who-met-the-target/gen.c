/* Random hours and a target that is, by the seed, 0, the largest allowed
 * value, one of the entries, one more than an entry, or random. Small
 * cases: up to 8 entries from 0 to 10, so entries equal to the target
 * come up often. Full size: mostly 50 entries, spread from 0 to 10^5 or
 * bunched within a few hours of the target (then 10^5 and 99999 come up
 * when the target is 10^5). */
void gen(int small)
{
    int hours[50];
    int n = small ? (int)rnd(1, 8) : (rnd_chance(80) ? 50 : (int)rnd(1, 50));
    int top = small ? 10 : 100000;
    int how = (int)rnd(0, 4), bunched = !small && rnd_chance(50);
    int target, i, at, v;

    if (how == 0)
        target = 0;
    else if (how == 1)
        target = top;
    else
        target = (int)rnd(0, top);
    for (i = 0; i < n; i++) {
        if (bunched) {
            v = target + (int)rnd(-3, 3);
            hours[i] = v < 0 ? 0 : v > top ? top : v;
        } else {
            hours[i] = (int)rnd(0, top);
        }
    }
    at = (int)rnd(0, n - 1);
    if (how == 2)
        target = hours[at];
    else if (how == 3)
        target = hours[at] < top ? hours[at] + 1 : top;
    put_ints(hours, n);
    put_int(target);
}
